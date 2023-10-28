#include "Machine.h"
#include <QTimer>

// 1.如果调用“读写寄存器或参数”时传入了有效的echo，那么最终会回调onActionFinished(int err, SignalEcho echo)函数，
//   err取值：0：无错误，-1：超时，其他：异常码
// 2.信号发送的顺序：1)reconn；2)regUpdated；3)disconn；4)回调onActionFinished函数
//   发送信号之前，已经更新了“本地寄存器缓存”和连接状态，因此可以在相应槽中直接访问“机器参数(MParam)”
// 3.所有对“寄存器或参数”的访问操作都必须在同一线程
// 4.“读写寄存器”和“读写参数”的区别：
// “读写寄存器”是静态的。“写寄存器”时没有参数的类型信息，将直接发送字节数组，所以使用“写参数”更方便
// “读写参数”会自动组合寄存器连续的参数，但不能存在寄存器冲突并且同时生效的参数
// 如果参数的“所占的寄存器区域”或“读写最大延时”会动态改变，需要动态组合，那么可以使用“读写参数”

Machine::Machine(int maddr, MachineComm *comm,
                 MachineParamList *mplist,
                 QObject *parent) :
    QObject(parent)
{
    onf_priority = true;
    n_resentcnt = 5;
    onf_linkfram[0] = 1;
    n_linkfram[0] = 0;
    for (int p = 1; p < P; p++)  {
        onf_linkfram[p] = 0;
        n_linkfram[p] = 0;
    }

    m_maddr = maddr;
    m_comm = comm;
    m_mplist = mplist;
    m_isconn = false;
    m_isfirstconn = true;

    m_comm->echom.create(m_echo, m_ecIds, this, "onResponse");
    m_comm->echom.create(m_echof, m_ecIds, this, "onResponse");

    p_cur = -1;
    for (int p = 0; p < P; p++) {
        p_cnt[p] = -1;
    }
    q_isenq = false;
    c_acti = 0;
    c_cnta = 0;
    c_cntf = 0;

    // firstReadMpslist是用来重新连接时读取的参数
    firstReadMpslist0 = m_mplist->firstReadMpslist;

    // 如果firstReadMpslist为空，将永远是连接成功的状态
    if (firstReadMpslist0.isEmpty()) {
        m_isconn = true;
        m_isfirstconn = false;
    }
    else {
        firstReadMpslist = firstReadMpslist0;
        QTimer::singleShot(0, this, SLOT(nextFirstReadMpslist()));
    }
}

Machine::~Machine()
{
    m_comm->clearRequest(m_echo, m_ecIds);
    m_comm->clearRequest(m_echof, m_ecIds);
    m_comm->echom.destroy(m_ecIds, this);
}

void Machine::read(const MRegRWInfo &info, const SignalEcho &echo, uint p)
{
    read(QList<MRegRWInfo>() << info, echo, p);
}

void Machine::read(const QList<MRegRWInfo> &list, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = ReadReg;
    a.echo = echo;
    a.listr = list;
    enqueue(a, p);
}

void Machine::write(const MRegRWInfo &info, const SignalEcho &echo, uint p)
{
    write(QList<MRegRWInfo>() << info, echo, p);
}

void Machine::write(const QList<MRegRWInfo> &list, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = WriteReg;
    a.echo = echo;
    a.listr = list;
    enqueue(a, p);
}

void Machine::writeNoRU(const MRegRWInfo &info, const SignalEcho &echo, uint p)
{
    writeNoRU(QList<MRegRWInfo>() << info, echo, p);
}

void Machine::writeNoRU(const QList<MRegRWInfo> &list, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = WriteRegNoRU;
    a.echo = echo;
    a.listr = list;
    enqueue(a, p);
}

void Machine::read(const MParam *mp, const SignalEcho &echo, uint p)
{
    read(QList<const MParam *>() << mp, echo, p);
}

void Machine::read(const QList<const MParam *> &mps, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = ReadParam;
    a.echo = echo;
    a.listm = mps;
    enqueue(a, p);
}

void Machine::write(const MParam *mp, const QVariant &value, const SignalEcho &echo, uint p)
{
    write(QList<const MParam *>() << mp, QVariantList() << value, echo, p);
}

void Machine::write(const QList<const MParam *> &mps, const QVariantList &values, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = WriteParam;
    a.echo = echo;
    a.listm = mps;
    a.listv = values;
    enqueue(a, p);
}

void Machine::writeNoRU(const MParam *mp, const QVariant &value, const SignalEcho &echo, uint p)
{
    writeNoRU(QList<const MParam *>() << mp, QVariantList() << value, echo, p);
}

void Machine::writeNoRU(const QList<const MParam *> &mps, const QVariantList &values, const SignalEcho &echo, uint p)
{
    Action a;
    a.type = WriteParamNoRU;
    a.echo = echo;
    a.listm = mps;
    a.listv = values;
    enqueue(a, p);
}

// 这个函数会重新获取echo，注意要在回调函数里判断echo是否相等
void Machine::clearAction(SignalEcho &echo, SignalEchoIds &ids)
{
    if (!echo.isValid())
        return;

    // 稍后将请求队列中跟旧echo有关的项移除掉
    QMetaObject::invokeMethod(this, "clearAction_p", Qt::QueuedConnection,
                              Q_ARG_SignalEcho(echo));
    // 重新获取echo，旧echo的回调将会无效
    echom.reCreate(echo, ids);
}

void Machine::clearAction_p(const SignalEcho &echo)
{
    // 判断队列里的echo
    for (int p = 0; p < P; p++) {
        for (int i = q_act[p].size() - 1; i >= 0; i--) {
            for (int j = q_act[p].at(i).size() - 1; j >= 0; j--) {
                if (q_act[p].at(i).at(j).echo == echo) {
                    q_act[p][i].removeAt(j);
                    echom.echoEmpty(echo);
                }
            }
        }
    }

    // 判断当前正在发送的echo，只有当前所有echo都相等才会清除请求
    int i;
    for (i = c_acti; i < c_act.size(); i++)
        if (c_act.at(i).echo != echo)
            break;
    if (i == c_act.size() && i != 0) {
        // 清除正在发送的请求，并更新已接收到的寄存器数据，然后处理下一个请求
        echom.echoEmpty(echo, c_act.size() - c_acti);
        m_comm->clearRequest(m_echo, m_ecIds);
        if (!regAreas.v.isEmpty()) {
            m_mplist->setData(regAreas, datas);
            emit regUpdated(regAreas);
            regAreas.v.clear();
            datas.clear();
        }
        nextProcess();
    }
}

/*
先将要发送的Action缓存到QList中，然后触发enqueueFinished事件，
最后在enqueueFinished函数里把缓存里的Action组成“连帧”并加入到队列中，
根据Action.n_linkfram的值是否相同来决定是否组成“连帧”。
在onf_linkfram[p]为1的情况下，在同一个事件循环里发送的Action全都会自动组成“连帧”
在适当的位置对onf_linkfram[p]和n_linkfram[p]赋值可以灵活的组成“连帧”
*/
void Machine::enqueue(Action &a, uint p)
{
    a.n_resentcnt = n_resentcnt;

    if (onf_linkfram[p])
        a.n_linkfram = n_linkfram[p];
    else {
        a.n_linkfram = ++n_linkfram[p];
        n_linkfram[p]++;
    }

    if (a.echo.isValid())
        echom.from(a.echo);

    q_enq[onf_priority ? p : 0].append(a);
    if (!q_isenq) {
        q_isenq = true;
        QMetaObject::invokeMethod(this, "enqueueFinished", Qt::QueuedConnection);
    }
}

void Machine::enqueueFinished()
{
    q_isenq = false;

    for (int p = 0; p < P; p++) {
        n_linkfram[p] = 0; // 实际上这个值是多少完全不重要
        if (q_enq[p].isEmpty())
            continue;

        QList<Action> list;
        list.append(q_enq[p].at(0));
        short n_linkfram_last = q_enq[p].at(0).n_linkfram;

        for (int i = 1; i < q_enq[p].size(); i++) {
            short n_linkfram = q_enq[p].at(i).n_linkfram;
            if (n_linkfram != n_linkfram_last) {
                n_linkfram_last = n_linkfram;
                q_act[p].enqueue(list);
                list.clear();
            }
            list.append(q_enq[p].at(i));
        }
        q_act[p].enqueue(list);
        q_enq[p].clear();
    }

    if (p_cur == -1 && !m_isfirstconn)
        nextProcess();
}

void Machine::nextProcess()
{
    bool loop = true;
    while (loop) {
        loop = false;
        c_cnta = 0;
        c_acti = 0;
        c_acnt.clear();
        c_act.clear();
        p_cur = -1;

        // 即使高优先级队列时时刻刻都有请求，低优先级队列的请求也会有机会得到处理
        for (int p = 0; p < P; p++) {
            if (p_cnt[p] == 0) {
                p_cnt[p] = -1;
                loop = true;
                continue;
            }

            if (q_act[p].isEmpty())
                continue;

            p_cur = p;
            if (p_cnt[p] == -1)
                p_cnt[p] = q_act[p].size();
            //qDebug("p[0]=%d, p[1]=%d", p_cnt[0], p_cnt[1]);
            p_cnt[p]--;

            c_act = q_act[p].dequeue();
            if (c_act.isEmpty()) {
                loop = true;
                break;
            }
            c_acnt.reserve(c_act.size());
            QList<MCRequest> reqs;

            for (int i = 0; i < c_act.size(); i++) {
                const Action &a = c_act.at(i);
                QList<MRegRWInfo> listr = genMRegRWInfo(a);
                c_acnt.append(listr.size());
                for (int j = 0; j < listr.size(); j++)
                    reqs.append(genMCRequest(listr.at(j), a));
            }

            if (m_isconn) {
                // c_cnta用于记录当前正在发送的Action里实际有多少帧要发送，只有所有帧都接收完成才算完
                // c_acti用于记录Qlist<Action>哪个是当前Action，只有所有Action都处理完才会处理下一个Qlist<Action>
                c_cnta = c_acnt.at(c_acti);
                m_comm->request(reqs, m_echo);
                return;
            }
            else {
                // 如果连接是断开的，将会直接把队列里所有的Action当作超时处理，但还是按正常的循环顺序来处理
                for (int i = 0; i < c_act.size(); i++) {
                    const Action &a = c_act.at(i);
                    if (a.echo.isValid()) {
                        echom.echo(a.echo,
                                   Q_ARG(int, -1),
                                   Q_ARG_SignalEcho(a.echo));
                    }
                }
                loop = true;
                break;
            }
        }
    }
}

QList<MRegRWInfo> Machine::genMRegRWInfo(const Machine::Action &a)
{
    if (a.type == ReadReg || a.type == WriteReg || a.type == WriteRegNoRU)
        return a.listr;
    int maxrgec = a.type == ReadParam ? 125 : 123;

    // 挑选参数
    QList<MRegRWInfo> list1;
    QListIterator<QVariant> valuei(a.listv);
    bool isWrite = (a.type == WriteParam || a.type == WriteParamNoRU);
    for (int i = 0; i < a.listm.size(); i++) {
        // 如果是写参数，值的个数小于参数个数的情况下，将以值的个数为准（值的个数不能为零）
        if (isWrite && !valuei.hasNext())
            break;
        const MParam *mp = a.listm.at(i);
        if (mp->owner() != m_mplist)
            qWarning("warning: MachineParam of other.");
        if (mp->isValid()) {
            MRegRWInfo r;
            r.addr = mp->getAddr();
            r.count = mp->getCount();
            r.rdelay = mp->getRDelay();
            r.wdelay = mp->getWDelay();
            if (isWrite)
                r.data = mp->regFormat(valuei.next());
            list1.append(r);
        }
    }

    // 组合参数
    std::sort(list1.begin(), list1.end(), lessThan);
    QList<MRegRWInfo> list2;
    MRegRWInfo reg = list1.at(0);
    for (int i = 1; i < list1.size(); i++) {
        const MRegRWInfo &r = list1.at(i);
        if (reg.addr + reg.count > r.addr)
            qWarning("warning: MachineParam register conflict.");
        if (reg.addr + reg.count == r.addr && reg.count + r.count <= maxrgec) {
            reg.count += r.count;
            reg.data.append(r.data);
            if (reg.rdelay < r.rdelay)
                reg.rdelay = r.rdelay;
            if (reg.wdelay < r.wdelay)
                reg.wdelay = r.wdelay;
            continue;
        }
        list2.append(reg);
        reg = r;
    }
    list2.append(reg);
    return list2;
}

MCRequest Machine::genMCRequest(const MRegRWInfo &r, const Action &a)
{
    MCRequest req;
    if (a.type == ReadReg || a.type == ReadParam) {
        req.timeoutms = r.rdelay;
        req.resendcnt = a.n_resentcnt;
        req.rsplen = 3 + r.count * 2;
        QByteArray ba(6, Qt::Uninitialized);
        char *data = ba.data();
        data[0] = m_maddr;
        data[1] = 0x03;
        data[2] = r.addr >> 8;
        data[3] = r.addr >> 0;
        data[4] = r.count >> 8;
        data[5] = r.count >> 0;
        req.data = ba;
    }
    else
    {
        req.timeoutms = r.wdelay;
        req.resendcnt = a.n_resentcnt;
        req.rsplen = 6;
        QByteArray ba(7 + r.data.length(), Qt::Uninitialized);
        char *data = ba.data();
        data[0] = m_maddr;
        data[1] = 0x10;
        data[2] = r.addr >> 8;
        data[3] = r.addr >> 0;
        data[4] = r.count >> 8;
        data[5] = r.count >> 0;
        data[6] = r.count * 2;
        memcpy(&data[7], r.data.constData(), r.data.length());
        req.data = ba;
    }
    return req;
}

void Machine::nextFirstReadMpslist()
{
    int resendcnt = firstReadMpslist.size() == firstReadMpslist0.size() && !m_isfirstconn ? 0 : 5;
    QList<const MParam *> mps = firstReadMpslist.takeFirst();

    Action a;
    a.type = ReadParam;
    a.listm = mps;

    QList<MCRequest> reqs;
    QList<MRegRWInfo> listr = genMRegRWInfo(a);
    for (int j = 0; j < listr.size(); j++) {
        MCRequest req = genMCRequest(listr.at(j), a);
        req.resendcnt = resendcnt;
        resendcnt = n_resentcnt;
        reqs.append(req);
    }

    c_cntf = listr.size();
    m_comm->request(reqs, m_echof);
}

void Machine::onResponse(int err,
                         const MCRequest &req,
                         const MCResponse &rsp,
                         const SignalEcho &echo)
{
    m_comm->echom.finish(echo);
    if (echo != m_echo && echo != m_echof)
        return;

    bool bDisconn = false;
    bool bReconn = false;
    bool bRegUpdate = false;
    bool bNextProcess = false;
    QList<SignalEcho> e;


    // 处理响应帧
    if (err == 0) {
        const char *data = req.data.constData();
        if (data[1] == 0x03) {
            int addr = ((uint)(uchar)data[2] << 8) | ((uint)(uchar)data[3]);
            int count = ((uint)(uchar)data[4] << 8) | ((uint)(uchar)data[5]);
            regAreas.v.append(MRegArea(addr, count));
            datas.append(QByteArray(&rsp.data.constData()[3], count*2));
        }
        else if (data[1]  == 0x10) {
            ActionType t = (echo == m_echof) ? WriteParam : c_act.at(c_acti).type;
            if (t != WriteRegNoRU && t != WriteParamNoRU) {
                const char *data = req.data.constData();
                int addr = ((uint)(uchar)data[2] << 8) | ((uint)(uchar)data[3]);
                int count = ((uint)(uchar)data[4] << 8) | ((uint)(uchar)data[5]);
                regAreas.v.append(MRegArea(addr, count));
                datas.append(QByteArray(&data[7], count*2));
            }
        }
    }


    if (echo == m_echof) {
        if (err != 0) { // 超时或收到异常帧
            // 重连失败
            if (m_isfirstconn) {
                // 首次连接失败
                m_isfirstconn = false;
                bDisconn = true;
                bNextProcess = true;
            }
            firstReadMpslist = firstReadMpslist0;
            QTimer::singleShot(1000, this, SLOT(nextFirstReadMpslist()));
            bRegUpdate = true;
        }
        else if (--c_cntf == 0) {
            if (firstReadMpslist.isEmpty()) {
                // 重连成功
                if (m_isfirstconn) {
                    // 首次连接成功
                    m_isfirstconn = false;
                    bNextProcess = true;
                }
                else
                    bReconn = true;

                m_isconn = true;
            }
            else
                // 重连中
                nextFirstReadMpslist();

            bRegUpdate = true;
        }
    }

    else if (echo == m_echo) {
        if (err == -1) { // 超时
            // 断开了连接
            if (!firstReadMpslist0.isEmpty()) {
                firstReadMpslist = firstReadMpslist0;
                QTimer::singleShot(1000, this, SLOT(nextFirstReadMpslist()));
                m_isconn = false;
                bDisconn = true;
            }
            for (; c_acti < c_act.size(); c_acti++)
                e.append(c_act.at(c_acti).echo);
            bNextProcess = true;
            bRegUpdate = true;
        }
        else  if (err != 0) { // 收到异常帧
            for (; c_acti < c_act.size(); c_acti++)
                e.append(c_act.at(c_acti).echo);
            bNextProcess = true;
            bRegUpdate = true;
        }
        else  if (--c_cnta == 0) { // 一个Action接收完成
            e.append(c_act.at(c_acti).echo);
            if (++c_acti < c_act.size())
                c_cnta = c_acnt.at(c_acti);
            else
                bNextProcess = true;
            bRegUpdate = true;
        }
    }


    // 发送信号
    if (bReconn)
        emit reconn();
    if (bRegUpdate && !regAreas.v.isEmpty()) {
        m_mplist->setData(regAreas, datas);
        emit regUpdated(regAreas);
        regAreas.v.clear();
        datas.clear();
    }
    if (bDisconn)
        emit disconn();
    for (int i = 0; i < e.size(); i++) {
        if (e.at(i).isValid()) {
            echom.echo(e.at(i),
                       Q_ARG(int, err),
                       Q_ARG_SignalEcho(e.at(i)));
        }
    }

    // 处理下一个
    if (bNextProcess)
        nextProcess();
}
