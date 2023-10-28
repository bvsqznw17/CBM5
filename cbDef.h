#ifndef CBDEF_H
#define CBDEF_H
#pragma pack(push)
#pragma pack(2)

#include <QtGlobal>

typedef qint8 int8;
typedef quint8 uint8;
typedef qint16 int16;
typedef quint16 uint16;
typedef qint32 int32;
typedef quint32 uint32;

typedef struct struct_PrmRec1{

    char 		name[20];			//产品名称	8
    int32		s32SetWeight[3];	//设置重量	12
    uint16		speed;				//包装速度	2
    uint16      bWorP;				//重量或包数,0:重量模式;1:按重量组合，目标值为包数,2:斗重量转换为包数再进行组合
    int16		s16PCL;				//偏差量	2
    uint16		DZJ_Z_ZF;			//主振机振幅	2
    uint16		DZJ_X_ZF[24];			//线振机振幅	1-MAX_DOU,0保留;法国客户定制版:0,振幅动态调节范围，//面条称DZJ_X_ZF[17]用作提升机目标斗数
    uint16		DZJ_Z_RunTime;		//				2
    uint16		DZJ_X_RunTime;		//线振机振动时间	2
    uint16		AFCMode;				//				1
    uint16		AFCT_TDS;			//目标斗数		1   64
    uint16		AFCT_Cnt;			//重置间隔		2
    int16		s16AFCT_LDS;			//限制斗数		2
    int16		s16AFCI_DDW;			//单斗重量		2
    uint16		AFCI_Cnt;			//				2
    int16		AFCI_LW;			//限制重量		2
    uint16      AFCR_Time;
    int16		DWW;				//单位重量
    int16		OverWeight;			//单斗重量超过该重量就排放(相当于超重)
    uint16 		P2Cnt;				//组合模式2时,自动调整单位重量, 调整间隔,

    uint16		WL_check;		//无料检出
    uint16		NoSel_check;	//无选择检出
    uint16		AutoZeroTime;	//自动置零时间
    uint16		LDJL_Time;		//料斗加料时间
    uint16		MFL_Cnt;		//多次放料次数
    uint16	    JLD_MFLNum;	//集料斗多次放料次数,在MFL_Cnt大于1时起作用
    uint16		HCD_Dly;		//缓冲斗延时
    uint16		CZD_Dly;		//称重斗延时
    uint16		JYD_Dly;		//记忆斗延时
    uint16		JLD_Dly;		//集料斗延时
    int16		JLD_OpenTime;	//集料斗开门时间(后延时)
    uint16		FL_dly;			//放料延时
    uint16		FL_time;		//放料信号时间        22
    uint16      MSV_Dly;		//称重稳定时间
    uint16		ZHFL_time;		//组合放料间隔
    uint16   	res;  		//法国客户定制:主振提升机调节比率
    uint16		Dou_PauseTime[4];  	//缓冲,称重,集料,记忆斗停顿时间

    uint16		TWW;	//JYD_PauseTime2 双汇改为单个皮重,//面条称用作提升机重置间隔
    uint16		ZZJ_QLTJ;		//缺料停机主振机下限比例
    int32		ZZJ_ULW;		//主振机上限重量
    int32		ZZJ_DLW;		//主振机下限重量
    int32    	CombinWeight;		//禁止组合重量，低于该重量的斗不参与组合，继续加料
    uint16  	MaDaMode[4]; 	//进料斗模式,称重斗模式,集料斗模式，//面条称记忆斗的MaDaMode[2]用作提升机限制斗数
    uint16  	ZZMDMode;  	//主振马达模式
    uint16      JYD_Dly2;		//QZPF_JL强制排放加料改为强排导管后延时
    uint16		ASF;		  	//滤波系数	               38

//#ifdef   USE_ASSIGN_DOU
    uint16		assignDou[10];
    int16		WeightUp[10];
    int16		WeightDn[10];
    uint16		SigMultSel; 			//单斗或多斗组合选择,每一位对应1组指定斗;1:指定斗多斗都可以参与组合，0:一次只有一个斗参与组合
    uint16		LowerQPSel; 			//低于WeightDn重量大于禁止组合重量强排，与SigMultSel互斥
//#endif

}PRMSET;

typedef  struct struct_SysSet{
    uint8  		CPUSN[8];
    uint16      type;  		//机器类型
    uint8   	Ver[12]; 	//系统软件版本
    uint8  		Date[8];
    uint16  	prmID[5];	//每组称的程序号
    uint16		SXBS;		//上限倍数
    uint16		YH_Combin;	//优化组合次数
    uint16		QZPF_DS;	//强排斗数
    uint16		JLD_state;	//集料斗状态
    uint16		LWJC_FS;	//料位检测方式  1:称重传感器  0:光电开关
    uint16		QP_2CJL;	//强排二次加料开关 0:关闭 1:打开
    uint16		dot_num;	//小数点位置
    uint16		TrackMode;	//触发模式
    uint16		FL_FS;		//放料方式  0:独立放料,1:配料(同时放料),2:轮流放料
    uint16		ASF;		//滤波系数
    uint16		DOU_State[32];	//斗状态
    char 		pwd1[8];
    char 		pwd2[8];
    char 		pwd3[8];
    uint16      YC_FLFS;			//依次放料方式 0~2
    uint16		Unit;				//单位
    uint16      ZDSense;			//振动传感器改为主振模式:0,电振机，1.电机驱动的转盘
    uint16      DZJ_ZFSet;			//电振机振幅
    uint16		DZJ_PLSet;			//电振机频率设置
    uint16      QXResetTime;		//清洗复位时间
    uint16 		RunMode;			// 0:单循环，1:双循环模式,2:异步
    uint16      AutoZeroMode;  	// 0:按时间置零，1:按组合次数置零
    uint16 		div;			//保留(保证与上位机结构体对齐) 双汇改为舍入限值
    uint16      QPDGSel;		//强排导管选择
    uint16		JLXHmode;		//加料信号模式 0.无外部加料信号,1.输出振机加料信号,2.外部数粒机信号
    int32 		AutoZeroWeight; //自动置零重量范围
    uint16		VibrationSen;	//振动传感器
    uint16      VibrationCoe;  	//振动系数0.500~1.500(500~1500)
    uint32  	nov_czd;		//称重斗的校正砝码重量
    uint32      nov_zzj;  		//主振的校正砝码重量
    uint32   	max_czd;		//称重斗的最大量程
    uint32   	max_zzj;		//主振的最大量程
    uint8 		BurdeningOpt[8];  //配料优先组(保证某一组的组合重量) A-D
    uint16  	RecordCounts[5];  //记录条数
    uint8   	ioin_pin[16]; 		//输入端口引脚设置
    uint8 		ioout_pin[32];		//输出端口引脚设置
    uint8  		DZJ_FreqSet[32];	//单个振机频率
    uint8  		DZJ_GainSet[32]; 	//单个振机增益
    uint16      ioin_status;
    uint32      ioout_status;
    uint8		bQXFlag[2];
}SYSSET;

typedef struct struct_PrmSetHead{
    char 		name[20];			//产品名称	8
    int32		s32SetWeight[3];	//设置重量	12
    uint16		speed;				//包装速度	2
    uint16      bWorP;				//重量或包数
}PRMSETHEAD;

typedef struct struct_ver_status{
     char ver[8];
     uint16  status[2];
}TVERSTATUS;

typedef struct struct_CombinStatus{
    int32		Weight;		//组合重量	12
    uint8 		cbtype;  	//组合类型
    uint8  		AvgDouNum;  	//平均斗数
    uint16		speed;				//包装速度	2
    uint16  	RunStatus;
    uint16      AlarmDou;   	//报警的斗
    uint16      CombinCount;
    uint16    	DWW;   			//组合模式2时自动调节的单位重量
    uint8      	DouStatus[10];				//
}TOLCOMBINSTATUS;


typedef  struct struct_modbus_para{
    //SYSSET  tSysSet;
    union
    {
        uint16  sysset[256];  	//系统设置固定为256个寄存器
        SYSSET  tSysSet;
    };

    uint16  PrmID;  	//寄存器中当前程序编号,高8位:1编辑程序号,0读
    union
    {
        uint16 	prmset[256];  	//程序设置固定为256个寄存器
        PRMSET  tPrmSet;
    };

    uint16  MotorID;
    uint16  MotorSet[120];

    uint16  PrmHeadID;
    PRMSETHEAD	tPrmHead[10];

    uint16  CmdPara;
    uint16  command;

    uint16  runstatus[70];

    uint16  doustatus[80];

    union
    {
        uint16  databuf[1024];
//        TMSVTESTBUF tSampTest;	  //采样测试
        TVERSTATUS  tVerSts[32];  //各模块的版本和状态
    };
}TMODBUSPARA;

//5代机电机驱动参数
typedef  struct struct_MOTORDRV{
    uint16  	pos;   	//距离/停止时间
    int16      	speed;   //最大速度 速度为0时，pos为暂停时间
    uint16		start;   //起始速度
    uint16  	end;	 //结束速度
    uint16    	acc;  	 //加速度
    uint16  	dec;   	 //减速度
}TMOTORDRV;

#pragma pack(pop)
#endif // CBDEF_H
