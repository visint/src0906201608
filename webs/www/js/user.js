function showUserNav(privilege,cp)
{
 var up,un,menu,page;
 if (privilege!=1)
 {
 	menu= new Array(6);
 	page= new Array(6);
 	menu[1]="�û��б�";
 	menu[2]="����û�";
 	menu[3]="�޸�����";
 	menu[4]="����Ȩ��";
 	menu[5]="����";
 	page[1]="otuca";
 	page[2]="otuddm";
 }
 else
 {
 	menu= new Array(3);
 	page= new Array(3);
 	menu[1]="�޸�����";
 	menu[2]="����Ȩ��";
 }
 menu[0]=un + " ��Ϣ";
 page[0]="user";
 showUnitNav(menu,page,un,up,cp);
}