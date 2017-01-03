function showUserNav(privilege,cp)
{
 var up,un,menu,page;
 if (privilege!=1)
 {
 	menu= new Array(6);
 	page= new Array(6);
 	menu[1]="用户列表";
 	menu[2]="添加用户";
 	menu[3]="修改密码";
 	menu[4]="访问权限";
 	menu[5]="保存";
 	page[1]="otuca";
 	page[2]="otuddm";
 }
 else
 {
 	menu= new Array(3);
 	page= new Array(3);
 	menu[1]="修改密码";
 	menu[2]="访问权限";
 }
 menu[0]=un + " 信息";
 page[0]="user";
 showUnitNav(menu,page,un,up,cp);
}