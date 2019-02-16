/* 首字hash函数1,针对gb2312拥有的99.5%的常用汉字的hash
输入: 汉字(分高低2字节)
输出: 该汉字的索引号
*/
int hashGb2312(const char* ch) {
	//检验是不是gb2312编码
	if (((ch[0] + 256) % 256 - 0xA0 < 16) || ((ch[0] + 256) % 256 - 0xA0 > 87))
		//gb2312汉字编码高位从第16区到第87区
		return -1;
	if (((ch[1] + 256) % 256 - 0xA0 < 1) || ((ch[1] + 256) % 256 - 0xA0 > 94))
		//gb2312汉字编码低位从1到94
		return -1;
	int sectionIndex = (ch[0] + 256) % 256 - 0xA0 - 16; //区号(基数为0),减16因为gb2312前15区没用，汉字区号从第16区开始
	int locationIndex = (ch[1] + 256) % 256 - 0xA0 - 1; //位号(基数为0),减1因为gb2312位号从1开始，希望从0开始，故减1

	int index = sectionIndex * 94 + locationIndex; //gb2312每区94个字符，这个保证hash的结果和区位码是一一对应的
	return index;
}

/*首字hash函数2,针对gb2312编码中没有出现的汉字的hash函数
输入: 汉字(分高低2字节)
输出: 该汉字的索引号
*/
int hashGbk(const char* ch) {
	int highIndex = (ch[0] + 256) % 256 - 0x81;
	int lowIndex;
	if ((ch[1] + 256) % 256 > 0x7f)
		lowIndex = (ch[1] + 256) % 256 - 0x40 - 1; //第二字节不能是0x7f，所以第二字节比0x7f大的再多减1，这样防止hash空白空间的浪费
	else
		lowIndex = (ch[1] + 256) % 256 - 0x40;

	int index = highIndex * 190 + lowIndex;
	return index;
}
