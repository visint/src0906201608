﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
   <title>OSS 通道1设置</title>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
   <link rel="stylesheet" href="css/style.css" type="text/css" />
   <link rel="stylesheet" href="css/button.css" type="text/css"/>
</head>
<script language="javascript">
//var slot=<%UnitID();%>;
function goOss()   
{ 
  parent.window.frames['topFrame'].location.href="/cv.asp";
  //alert("/goform/unit?cid=0&uid=" +document.getElementById("uid").value +"&page=" +p);
  location.href="/goform/slot?cid=0&uid=" +<%UnitID();%> +"&page=oss1_2";
}

function toUrl(p)   
{ 
  //parent.window.frames['topFrame'].location.href="/cv.asp";
  location.href="/goform/slot?cid=0&uid=<%UnitID();%>&page=" +p;   
}

</script>
<body>
<ul id="nav">
    <li><span>OSS=></span></li>
    <li><a href="#" id="base" name="navitem" onclick="javascript:toUrl('ossbi');">基本信息</a></li>
    <li><a href="#" id="oss" name="navitem" onclick="javascript:toUrl('oss');">OSS 通道1信息</a></li>
    <li><a href="#" id="oss1_2" name="navitem" class="current" onclick="javascript:toUrl('oss1_2');">OSS 通道2信息</a></li>
    <!--<li><a href="../help/oss.htm" target="new">帮助</a></li>-->
  </ul>
<div class="cbox vst10">
<div id="ibox">设置完毕</div>
<input class="button white medium" type="button" value="返回" onclick="goOss();" name="refresh"/>
</div>
</body>
</html>

