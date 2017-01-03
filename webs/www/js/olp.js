function showOlpNav(ut,cp)
{
var up,un,menu= new Array(1),page= new Array(1);
 if (ut<50)
 {
 	  un="OLP";
 }
 else
 {
 	  un="OBP";
 }
  menu[0]=un + "信息";
  page[0]="olp";
  showUnitNav(menu,page,un,"",cp);
}
function showOlpInfo(olp,utype)
{
	var rule,rule_item,item_count=15;
	
	if (!(utype==2||utype==21))
	{
	 	 rule="切换规则";
	   rule_items=[[1,"工作光纤优先"],[2,"光功率大优先"]];
	}
	else if (utype!=20)
	{
		rule="回切到主路";
		rule_items=[[1,"回切到主路"],[2,"不回切"]];
	}
	var head=new Array();
  var values=new Array();
  var m=2;

  var wl=[[1,"1310"],[2,"1550"]];
  head[0]=["工作模式",8];
  head[1]=["工作线路",9];
  if (utype!=2)
  {
  	head[m++]=["返回模式",11];
  	head[m++]=["返回时间",7];
  }
  if (utype==3)
    head[m++]=["主从",18];
  if (utype!=21)
   head[m++]=[rule,10];
  if (!(utype==2||utype==21))
  {
    head[m++]=["L1切换功率",4];
    head[m++]=["L2切换功率",5];
  }
  else
  {
    head[m++]=["主纤业务切换功率",4];
    head[m++]=["备纤业务切换功率",5];
    head[m++]=["主纤备光切换功率",21];
    head[m++]=["备纤备光切换功率",22];
  }
  head[m++]=["Tx告警功率",6];
  if (utype==13)
    head[m++]=["RX光功率",0];
  head[m++]=["R1光功率",0];
  head[m++]=["R2光功率",0];
  
  head[m++]=["TX光功率",0];
  if (utype==13)
 	{
    head[m++]=["T1光功率",0];
    head[m++]=["T2光功率",0];
     head[m++]=["T2‐1490 功率",0];
 	}

  head[m++]=["R1波长",15];
  head[m++]=["R2波长",16];
  head[m++]=["TX波长",17];
  if (utype==13)
  {
  	head[m++]=["隔离度",24];
  }
  if (utype==21)
  {
  	head[m++]=["备纤波长",19];
  	head[m++]=["备纤光功率",0];
  	head[m++]=["主从",18];
  }
  m=2;
  values[0]=getSelectInput([[1,"自动"],[2,"手动"]],8,olp.mode,"onchange=\"onItemChanged(this);\"");
  values[1]=getSelectInput([[1,"光开关在主路(L1)"],[2,"光开关在备路(L2)"]],9,olp.line,null);

  //values[2]=getSelectInput([[1,"自动"],[2,"手动"]],11,olp.ret_mode,null);
  if (utype!=2)
  {
  	values[m++]=getSelectInput([[1,"自动"],[2,"手动"]],11,olp.ret_mode,null);
  	values[m++]=getDecInput(7,olp.ret_time,1,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckInt(this,0,999);'")+"分钟";
  }
  if (utype==3)
    values[m++]=getSelectInput([[1,"主"],[2,"从"]],18,olp.master,null);
  if (utype!=21)
  values[m++]=getSelectInput(rule_items,10,olp.rule,true);

  values[m++]=getDecInput(4,(olp.l_switch_power[0]*0.1).toFixed(1),10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  values[m++]=getDecInput(5,(olp.l_switch_power[1]*0.1).toFixed(1),10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  if (utype==21)
  {
  	values[m++]=getDecInput(21,(olp.power_switch[0]*0.1).toFixed(1),10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
    values[m++]=getDecInput(22,(olp.power_switch[1]*0.1).toFixed(1),10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  }
  values[m++]=getDecInput(6,(olp.tx_alm_power*0.1).toFixed(1),10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  if (utype==13)
    values[m++]=(olp.RxPower*0.1).toFixed(1)+" dBm";
  values[m++]=(olp.rx_power[0]*0.1).toFixed(1)+" dBm";
  values[m++]=(olp.rx_power[1]*0.1).toFixed(1)+" dBm";

  values[m++]=(olp.tx_power*0.1).toFixed(1) +" dBm";
  if (utype==13)
  {
  	//values[m++]=(olp.rx_power[0]*0.1).toFixed(1)+" dBm";
    //values[m++]=(olp.rx_power[1]*0.1).toFixed(1)+" dBm";
    values[m++]=(olp.t1_power*0.1).toFixed(1)+" dBm";
    values[m++]=(olp.t2_power*0.1).toFixed(1)+" dBm";
    values[m++]=(olp.t2_power2*0.1).toFixed(1) +" dBm"
  }
  
  values[m++]=getSelectInput(wl,15,olp.rx_wave_len[0],null) +" nm";
  values[m++]=getSelectInput(wl,16,olp.rx_wave_len[1],null) +" nm";
  values[m++]=getSelectInput(wl,17,olp.tx_wave_len,null) + " nm";
  if (utype==13)
  {
  	values[m++]=getSelectInput([[1,"常规"],[2,"隔离度"]],24,olp.isolation,null);
  }
  if (utype==21)
  {
  	values[m++]=getSelectInput(wl,19,olp.bf_wave_len,null) +" nm";
  	values[m++]=(olp.bf_power*0.1).toFixed(1) +" dBm";
  	values[m++]=getSelectInput([[1,"主"],[2,"从"]],18,olp.master,null);
  }
 
  writeTableBody(head,values,2);
}
function showObpInfo(obp)
{
  var head=[["工作模式",10],["工作线路",11],["返回模式",13],["返回时间",9],["切换规则",12],["R1切换功率",5],["R2切换功率",6],["R3告警功率",7],["R4告警功率",8],["R1光功率",0],["R2光功率",0],["R3光功率",0],["R4光功率",0],["R1波长",0],["R2波长",0],["R3波长",0],["R4波长",0]];
  var values=[17];
  var wl=[[3,"850"],[1,"1310"],[2,"1550"]];
  var line= new Array(2);
  line[0]= new Object();
  line[0].name="光开关在主路";
  line[0].val=1;
  line[1]= new Object();
  line[1].name="光开关在旁路";
  line[1].val=2;
  
  values[0]=getSelectInput([[1,"自动"],[2,"手动主路"],[3,"手动旁路"],[4,"自动+心跳"]],10,obp.mode,null);
  values[1]=getItemName(line,obp.line);
  values[2]=getSelectInput([[1,"自动"],[2,"手动"]],13,obp.ret_mode,null);
  values[3]=getDecInput(9,obp.ret_time,1,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckInt(this,0,999);'")+"分钟";
  values[4]=getSelectInput([[0,"R1和R2 无光切换"],[1,"R1 无光切换"],[2,"R2 无光切换"],[3,"R1或R2无光切换"]],12,obp.rule,true);
  values[5]=getDecInput(5,obp.l_switch_power[0]*0.1,10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  values[6]=getDecInput(6,obp.l_switch_power[1]*0.1,10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  values[7]=getDecInput(7,obp.rx_alm_power[0]*0.1,10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  values[8]=getDecInput(8,obp.rx_alm_power[1]*0.1,10,3,5,2,"onkeyup='inputNumber(this,1,true);' onblur='return onCheckFloat(this,-50,20);'")+"dBm";
  for (n=0;n<4;n++)
   values[9+n]=(obp.rx_power[n]*0.1).toFixed(1) +" dBm";
  for (n=0;n<4;n++)
   values[13+n]=getSelectInput(wl,18+n,obp.rx_wave_len[n],null) +"nm";
	writeTableBody(head,values,3);
}