#ifndef MACHINEPARAMLIST0_H
#define MACHINEPARAMLIST0_H

#include "MachineParamList.h"

class MachineParamList0 : public MachineParamList
{
public:
    struct MMParams
    {
    // auto code
    QList<const MParam *>                      First; /* {{m.sys_CPUID,m._sys_end},{m.prm_id,m._prm_end}} */
    QList<MRegRWInfo>                            Sys; /* {{m.sys_CPUID,m._sys_end}} */
    QList<MRegRWInfo>                           Sys0; /* {{m.sys_CPUID,m.sys_VibrationCoe}} */
    QList<MRegRWInfo>                          Calib; /* {{m.sys_nov_czd,m.sys_max_zzj}} */
    QList<MRegRWInfo>                         IO_Set; /* {{m.sys_ioin_pin,m.sys_ioout_pin}} */
    QList<MRegRWInfo>                      IO_Status; /* {{m.sys_ioin_status,m.sys_ioout_status}} */
    QList<MRegRWInfo>                           PLZY; /* {{m.sys_DZJ_FreqSet,m.sys_DZJ_GainSet}} */
    QList<MRegRWInfo>                            Prm; /* {{m.prm_id,m._prm_end}} */
    QList<MRegRWInfo>                          Motor; /* {{m.motor_id,m._motor_end}} */
    QList<MRegRWInfo>                         Motor2; /* {{m.motor2_id,m._motor2_end}} */
    QList<MRegRWInfo>                        PrmList; /* {{m.prmlist_id,m._prmlist_end}} */
    QList<MRegRWInfo>                            Mod; /* {{m.mod_totaln,m._mod_end}} */
    QList<MRegRWInfo>                           Samp; /* {{m.samp_dounum,m._samp_end}} */
    QList<MRegRWInfo>                         ZdTest; /* {{m.zdtest_dou_weight,m._zdtest_end}} */
    QList<MRegRWInfo>                    sys_prm_ids;
    QList<MRegRWInfo>                  sys_DOU_State;
    QList<MRegRWInfo>               sys_RecordCounts;
    QList<MRegRWInfo>                   prm_DZJ_X_ZF;
    QList<MRegRWInfo>              prm_Dou_PauseTime;
    QList<MRegRWInfo>                   prm_MaDaMode;
    QList<MRegRWInfo>                    prm_ZDD_Dou;
    QList<MRegRWInfo>               prm_ZDD_WeightUp;
    QList<MRegRWInfo>               prm_ZDD_WeightDn;
    QList<MRegRWInfo>                      motor_pos;
    QList<MRegRWInfo>                    motor_speed;
    QList<MRegRWInfo>                    motor_start;
    QList<MRegRWInfo>                      motor_end;
    QList<MRegRWInfo>                      motor_acc;
    QList<MRegRWInfo>                      motor_dec;
    QList<MRegRWInfo>                 motor_stoptime;
    QList<MRegRWInfo>                      motor2_sp;
    QList<MRegRWInfo>                      motor2_ta;
    QList<MRegRWInfo>                      motor2_tm;
    QList<MRegRWInfo>                      motor2_tb;
    QList<MRegRWInfo>                      motor2_ts;
    QList<MRegRWInfo>                   prmlist_name;
    QList<MRegRWInfo>              prmlist_SetWeight;
    QList<MRegRWInfo>             prmlist_SetWeight1;
    QList<MRegRWInfo>             prmlist_SetWeight2;
    QList<MRegRWInfo>                  prmlist_speed;
    QList<MRegRWInfo>                   prmlist_WorP;
    QList<MRegRWInfo>                     cmb_Weight;
    QList<MRegRWInfo>                     cmb_cbtype;
    QList<MRegRWInfo>                      cmb_speed;
    QList<MRegRWInfo>                  cmb_RunStatus;
    QList<MRegRWInfo>                   cmb_AlarmDOu;
    QList<MRegRWInfo>                cmb_combinCount;
    QList<MRegRWInfo>                        cmb_DWW;
    QList<MRegRWInfo>                  cmb_DouStatus;
    QList<MRegRWInfo>                    cmb_Weight2;
    QList<MRegRWInfo>               cmb_combinCount2;
    QList<MRegRWInfo>                      doustatus;
    QList<MRegRWInfo>                        mod_ver;
    QList<MRegRWInfo>                    mod_status0;
    QList<MRegRWInfo>                    mod_status1;
    QList<MRegRWInfo>                      samp_data;
    };
    struct MParams
    {
    // auto code
    MParam*                                sys_union; /*     0 Bytes  系统 */
    MParam*                                prm_union; /*   256 Bytes  程序 */
    MParam*                              motor_union; /*   513 Bytes  马达模式 */
    MParam*                            prmlist_union; /*   634 Bytes  程序列表 */
    MParam*                                cmd_param; /*   815 U16    命令参数 */
    MParam*                                      cmd; /*   816 U16    命令字 */
    MParam*                          runstatus_union; /*   817 Bytes  运行状态 */
    MParam*                          doustatus_union; /*   902 Bytes  斗状态 */
    MParam*                            databuf_union; /*   982 Bytes  其他数据 */
    MParam*                                sys_CPUID; /*     0 Bytes  CPU ID */
    MParam*                              sys_MacType; /*     4 U16    机器类型 */
    MParam*                               sys_MacVer; /*     5 Chars  固件版本 */
    MParam*                              sys_SysTime; /*    11 Bytes  系统时间 */
    MParam*                           sys_prm_ids[5]; /*    15 U16    每组称的程序号 */
    MParam*                                 sys_SXBS; /*    20 U16    上限倍数 */
    MParam*                            sys_YH_Combin; /*    21 U16    优化组合次数 */
    MParam*                              sys_QZPF_DS; /*    22 U16    强排斗数  //最大值为每组斗个数(*2) */
    MParam*                            sys_JLD_state; /*    23 U16    集料斗状态(集料斗选择) */
    MParam*                              sys_LWJC_FS; /*    24 U16    主振传感器 //料位检测方式  1:称重传感器  0:光电开关 */
    MParam*                              sys_QP_2CJL; /*    25 U16    强排二次加料开关 0:关闭 1:打开 */
    MParam*                              sys_dot_num; /*    26 U16    小数点位置 */
    MParam*                            sys_TrackMode; /*    27 U16    触发模式 */
    MParam*                                sys_FL_FS; /*    28 U16    放料方式  0:独立放料,1:配料(同时放料),2:轮流放料 */
    MParam*                                  sys_ASF; /*    29 U16    滤波系数 */
    MParam*                        sys_DOU_State[32]; /*    30 U16    斗状态(斗使能) */
    MParam*                                  sys_FDZ; /*    62 U16    分度值 */
    MParam*                               sys_nov_zd; /*    63 U16    振动传感器的校正砝码重量 */
    MParam*                               sys_max_zd; /*    64 U16    振动传感器的最大量程 */
    MParam*                               sys_PLCJMS; /*    65 U16    配料从机模式 */
    MParam*                                  _unused; /*    66 Bytes  保留 */
    MParam*                              sys_YC_FLFS; /*    74 U16    依次放料方式 */
    MParam*                                 sys_Unit; /*    75 U16    单位 */
    MParam*                              sys_ZDSense; /*    76 U16    振动传感器改为主振模式:0,电振机，1.电机驱动的转盘 */
    MParam*                            sys_DZJ_ZFSet; /*    77 U16    电振机振幅(改为振机增益)当未输入特殊密码时，最大值为2，否则为3 */
    MParam*                            sys_DZJ_PLSet; /*    78 U16    电振机频率 */
    MParam*                          sys_QXResetTime; /*    79 U16    清洗复位时间 */
    MParam*                              sys_RunMode; /*    80 U16    运行模式  0:单循环，1:双循环模式,2:异步 */
    MParam*                         sys_AutoZeroMode; /*    81 U16    自动置零模式  0:按时间置零，1:按组合次数置零 */
    MParam*                                  sys_div; /*    82 U16    进一下限 */
    MParam*                              sys_QPDGSel; /*    83 U16    强排导管选择 */
    MParam*                             sys_JLXHmode; /*    84 U16    加料信号模式 0.无外部加料信号,1.输出振机加料信号,2.外部数粒机信号 */
    MParam*                       sys_AutoZeroWeight; /*    85 S32    自动置零重量范围 */
    MParam*                         sys_VibrationSen; /*    87 U16    振动传感器 */
    MParam*                         sys_VibrationCoe; /*    88 U16    振动系数0.500~1.500(500~1500) */
    MParam*                              sys_nov_czd; /*    89 U32    称重斗的校正砝码重量 */
    MParam*                              sys_nov_zzj; /*    91 U32    主振的校正砝码重量 */
    MParam*                              sys_max_czd; /*    93 U32    称重斗的最大量程 */
    MParam*                              sys_max_zzj; /*    95 U32    主振的最大量程 */
    MParam*                         sys_BurdeningOpt; /*    97 Bytes  配料优先组(保证某一组的组合重量) A-D */
    MParam*                      sys_RecordCounts[5]; /*   101 U32    记录条数 */
    MParam*                             sys_ioin_pin; /*   106 Bytes  输入端口引脚设置 */
    MParam*                            sys_ioout_pin; /*   114 Bytes  输出端口引脚设置 */
    MParam*                          sys_DZJ_FreqSet; /*   130 Bytes  单个振机频率 */
    MParam*                          sys_DZJ_GainSet; /*   146 Bytes  单个振机增益 */
    MParam*                          sys_ioin_status; /*   162 U16    输入端口状态 */
    MParam*                         sys_ioout_status; /*   163 U32    输出端口状态 */
    MParam*                              sys_bQXFlag; /*   165 U16    清洗标志 */
    MParam*                                 _sys_end; /*   166 Bytes  _sys_end */
    MParam*                                   prm_id; /*   256 U16    当前程序 */
    MParam*                                 prm_name; /*   257 Chars  产品名称 */
    MParam*                            prm_SetWeight; /*   267 S32    设置重量目标值 */
    MParam*                           prm_SetWeight1; /*   269 S32    设置重量上限值 */
    MParam*                           prm_SetWeight2; /*   271 S32    设置重量下限值 */
    MParam*                                prm_speed; /*   273 U16    包装速度 */
    MParam*                                 prm_WorP; /*   274 U16    组合模式：重量或包数,0:重量模式;1:按重量组合，目标值为包数,2:斗重量转换为包数再进行组合 */
    MParam*                                  prm_PCL; /*   275 S16    偏差量 */
    MParam*                             prm_DZJ_Z_ZF; /*   276 U16    主振机振幅（主振模式为1时是主振延时） */
    MParam*                         prm_DZJ_X_ZF[22]; /*   277 U16    线振机振幅  1-MAX_DOU,0保留;法国客户定制版:0,振幅动态调节范围，//面条称DZJ_X_ZF[17]用作提升机目标斗数 */
    MParam*                          prm_DWW_UpLimit; /*   299 S16    单位重量上限 */
    MParam*                        prm_DWW_DownLimit; /*   300 S16    单位重量下限 */
    MParam*                        prm_DZJ_Z_RunTime; /*   301 U16    主振机振动时间 */
    MParam*                        prm_DZJ_X_RunTime; /*   302 U16    线振机振动时间 */
    MParam*                              prm_AFCMode; /*   303 U16    AFC模式 */
    MParam*                             prm_AFCT_TDS; /*   304 U16    AFCT目标斗数 */
    MParam*                             prm_AFCT_Cnt; /*   305 U16    AFCT重置间隔 */
    MParam*                             prm_AFCT_LDS; /*   306 S16    AFCT限制斗数 */
    MParam*                             prm_AFCI_DDW; /*   307 S16    AFCI目标重量 */
    MParam*                             prm_AFCI_Cnt; /*   308 U16    AFCI重置间隔 */
    MParam*                              prm_AFCI_LW; /*   309 U16    AFCI限制重量 */
    MParam*                            prm_AFCR_Time; /*   310 U16    AFCR偏差时间 */
    MParam*                                  prm_DWW; /*   311 S16    单位重量 */
    MParam*                           prm_OverWeight; /*   312 S16    单斗强排上限 //单斗重量超过该重量就排放(相当于超重) */
    MParam*                                prm_P2Cnt; /*   313 U16    单位重量重置间隔 //组合模式2时才使用,自动调整单位重量, 调整间隔, */
    MParam*                             prm_WL_check; /*   314 U16    无料检出 */
    MParam*                          prm_NoSel_check; /*   315 U16    无选择检出 */
    MParam*                         prm_AutoZeroTime; /*   316 U16    自动置零时间 */
    MParam*                            prm_LDJL_Time; /*   317 U16    料斗加料时间 */
    MParam*                              prm_MFL_Cnt; /*   318 U16    多次放料次数 */
    MParam*                           prm_JLD_MFLNum; /*   319 U16    集料斗多次放料次数,在MFL_Cnt大于1时起作用 */
    MParam*                              prm_HCD_Dly; /*   320 U16    缓冲斗延时 */
    MParam*                              prm_CZD_Dly; /*   321 U16    称重斗延时 */
    MParam*                              prm_JYD_Dly; /*   322 U16    记忆斗延时 */
    MParam*                              prm_JLD_Dly; /*   323 U16    集料斗延时 */
    MParam*                         prm_JLD_OpenTime; /*   324 S16    集料斗开门时间(后延时) */
    MParam*                               prm_FL_dly; /*   325 U16    放料延时 */
    MParam*                              prm_FL_time; /*   326 U16    放料信号时间 */
    MParam*                              prm_MSV_Dly; /*   327 U16    称重稳定时间 */
    MParam*                            prm_ZHFL_time; /*   328 U16    组合放料间隔(依次放料间隔时间1)(WM依次放料间隔时间) */
    MParam*                                  prm_res; /*   329 U16    法国客户定制:主振提升机调节比率 */
    MParam*                     prm_Dou_PauseTime[4]; /*   330 U16    缓冲,称重,集料,记忆斗停顿时间 */
    MParam*                                  prm_TWW; /*   334 U16    单个皮重 //JYD_PauseTime2 双汇改为单个皮重,//面条称用作提升机重置间隔 */
    MParam*                             prm_ZZJ_QLTJ; /*   335 U16    缺料停机主振机下限比例 */
    MParam*                              prm_ZZJ_ULW; /*   336 S32    主振机上限重量 */
    MParam*                              prm_ZZJ_DLW; /*   338 S32    主振机下限重量 */
    MParam*                         prm_CombinWeight; /*   340 S32    禁止组合重量，低于该重量的斗不参与组合，继续加料 */
    MParam*                          prm_MaDaMode[4]; /*   342 U16    进料斗模式,称重斗模式,集料斗模式，//面条称记忆斗的MaDaMode[2]用作提升机限制斗数 */
    MParam*                             prm_ZZMDMode; /*   346 U16    主振马达模式 */
    MParam*                             prm_JYD_Dly2; /*   347 U16    QZPF_JL强制排放加料改为强排导管后延时 */
    MParam*                                  prm_ASF; /*   348 U16    滤波系数 */
    MParam*                          prm_ZDD_Dou[10]; /*   349 U16    指定斗 */
    MParam*                     prm_ZDD_WeightUp[10]; /*   359 S16    指定斗重量上限 */
    MParam*                     prm_ZDD_WeightDn[10]; /*   369 S16    指定斗重量下限 */
    MParam*                          prm_ZDD_MultSel; /*   379 U16    指定斗多斗组合 //单斗或多斗组合选择,每一位对应1组指定斗;1:指定斗多斗都可以参与组合，0:一次只有一个斗参与组合 */
    MParam*                       prm_ZDD_LowerQPSel; /*   380 U16    指定斗超轻强排 //低于WeightDn重量大于禁止组合重量强排，与SigMultSel互斥 */
    MParam*                                 _prm_end; /*   381 Bytes  _prm_end */
    MParam*                          prm_AutoZeroCnt; /*   316 U16    自动置零次数 */
    MParam*                                 motor_id; /*   513 U16    当前马达模式 */
    MParam*                            motor_pos[10]; /*   514 S16    距离/暂停时间 */
    MParam*                          motor_speed[10]; /*   515 U16    最大速度 速度为0时，pos为暂停时间 */
    MParam*                          motor_start[10]; /*   516 U16    起始速度 */
    MParam*                            motor_end[10]; /*   517 U16    结束速度 */
    MParam*                            motor_acc[10]; /*   518 U16    加速度 */
    MParam*                            motor_dec[10]; /*   519 U16    减速度 */
    MParam*                             motor_gdtzjd; /*   574 S16    光电停止角度 */
    MParam*                            motor_wgdtzjd; /*   575 S16    无光电停止角度 */
    MParam*                               _motor_end; /*   576 Bytes  _motor_end */
    MParam*                       motor_stoptime[10]; /*   514 S16    暂停时间 */
    MParam*                                motor2_id; /*   513 U16    当前马达模式 */
    MParam*                            motor2_sp[10]; /*   514 S16    速度 */
    MParam*                            motor2_ta[10]; /*   515 U16    加速时间 */
    MParam*                            motor2_tm[10]; /*   516 U16    匀速时间 */
    MParam*                            motor2_tb[10]; /*   517 U16    减速时间 */
    MParam*                            motor2_ts[10]; /*   518 U16    延时 */
    MParam*                              _motor2_end; /*   564 Bytes  _motor2_end */
    MParam*                               prmlist_id; /*   634 U16    程序列表起始 */
    MParam*                         prmlist_name[10]; /*   635 Chars  产品名称 */
    MParam*                    prmlist_SetWeight[10]; /*   645 S32    设置重量目标值 */
    MParam*                   prmlist_SetWeight1[10]; /*   647 S32    设置重量上限值 */
    MParam*                   prmlist_SetWeight2[10]; /*   649 S32    设置重量下限值 */
    MParam*                        prmlist_speed[10]; /*   651 U16    包装速度 */
    MParam*                         prmlist_WorP[10]; /*   652 U16    重量或包数,0:重量模式;1:按重量组合，目标值为包数,2:斗重量转换为包数再进行组合 */
    MParam*                             _prmlist_end; /*   815 Bytes  _prmlist_end */
    MParam*                            cmb_Weight[5]; /*   817 S32    组合重量 */
    MParam*                            cmb_cbtype[5]; /*   819 U16    组合类型|平均斗数（低8位|高8位） */
    MParam*                             cmb_speed[5]; /*   820 U16    包装速度 */
    MParam*                         cmb_RunStatus[5]; /*   821 U16    运行状态 */
    MParam*                          cmb_AlarmDOu[5]; /*   822 U32    报警的斗 */
    MParam*                       cmb_combinCount[5]; /*   824 U16    组合计数 */
    MParam*                               cmb_DWW[5]; /*   825 S16    单位重量 */
    MParam*                         cmb_DouStatus[5]; /*   826 Bytes  斗状态 */
    MParam*                           cmb_Weight2[5]; /*   831 S32    输出组合重量 */
    MParam*                      cmb_combinCount2[5]; /*   833 U16    输出组合计数 */
    MParam*                                 _cmb_end; /*   902 Bytes  _cmb_end */
    MParam*                            doustatus[80]; /*   902 S16    斗状态 */
    MParam*                               mod_totaln; /*   982 U16    模块数量 */
    MParam*                              mod_ver[35]; /*   983 Chars  模块版本 */
    MParam*                          mod_status0[35]; /*   987 U16    模块状态 */
    MParam*                          mod_status1[35]; /*   988 U16    模块状态 */
    MParam*                                 _mod_end; /*  1193 Bytes  _mod_end */
    MParam*                              samp_dounum; /*   982 U16    采样斗编号 */
    MParam*                                samp_time; /*   983 U16    采样时间 */
    MParam*                                 samp_max; /*   984 S16    采样最大值 */
    MParam*                              samp_weight; /*   985 S16    采样重量值 */
    MParam*                            samp_datasize; /*   986 U16    采样数据个数 */
    MParam*                            samp_data[10]; /*   987 S16s   采样数据 */
    MParam*                                _samp_end; /*  1987 Bytes  _samp_end */
    MParam*                        zdtest_dou_weight; /*   982 S16    斗重量 */
    MParam*                         zdtest_zd_weight; /*   983 S16    振动重量 */
    MParam*                               zdtest_cpt; /*   984 U16    计算更新 */
    MParam*                              _zdtest_end; /*   985 Bytes  _zdtest_end */
    MParam*                                     _end; /*  2006 Bytes  _end */
    };

public:
    MachineParamList0();
    ~MachineParamList0();
    MMParams mm;
    MParams m;

    MParam *mps[656];
    void initMMParam(QList<const MParam *> &mmp, MParam *mmp2[][2], int len);
    void initMMParam(QList<MRegRWInfo> &mmp, MParam *mmp2[][2], int len);
};

#endif // MACHINEPARAMLIST0_H
