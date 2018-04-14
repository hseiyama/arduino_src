//*******************************************************************************
//----------------本例程仅供学习使用，未经作者允许，不得用于其他用途。-----------
//------------------------版权所有，仿冒必究！-----------------------------------
//----------------1.开发环境:Arduino IDE-----------------------------------------
//----------------2.测试使用开发板型号：Arduino Leonardo or Arduino UNO R3-------
//----------------3.单片机使用晶振：16M------------------------------------------
//----------------4.淘宝网址：http://xmdzpj.taobao.com---------------------------
//----------------5.作者：真红炎神---------------------------------------------
//*******************************************************************************/

int potpin=A0;//定义模拟接口7
int ledpin=13;//定义数字接口13（PWM 输出）
int ledpin1=11;//定义数字接口11（PWM 输出）
int ledpin2=10;//定义数字接口10（PWM 输出）


int val=0;// 暂存来自传感器的变量数值
void setup()
{
Serial.begin(9600);//设置波特率为9600
//注意：模拟接口自动设置为输入
}
void loop()
{
val=analogRead(potpin);// 读取传感器的模拟值并赋值给val
Serial.println(val);//显示val 变量
analogWrite(ledpin,val/4);// 打开LED 并设置亮度（PWM 输__________出最大值255）
analogWrite(ledpin1,val/4);// 打开LED 并设置亮度（PWM 输__________出最大值255）
analogWrite(ledpin2,val/4);// 打开LED 并设置亮度（PWM 输__________出最大值255）

delay(100);//延时0.01 秒
}

