/* ����hash����1,���gb2312ӵ�е�99.5%�ĳ��ú��ֵ�hash
����: ����(�ָߵ�2�ֽ�)
���: �ú��ֵ�������
*/
int hashGb2312(const char* ch) {
	//�����ǲ���gb2312����
	if (((ch[0] + 256) % 256 - 0xA0 < 16) || ((ch[0] + 256) % 256 - 0xA0 > 87))
		//gb2312���ֱ����λ�ӵ�16������87��
		return -1;
	if (((ch[1] + 256) % 256 - 0xA0 < 1) || ((ch[1] + 256) % 256 - 0xA0 > 94))
		//gb2312���ֱ����λ��1��94
		return -1;
	int sectionIndex = (ch[0] + 256) % 256 - 0xA0 - 16; //����(����Ϊ0),��16��Ϊgb2312ǰ15��û�ã��������Ŵӵ�16����ʼ
	int locationIndex = (ch[1] + 256) % 256 - 0xA0 - 1; //λ��(����Ϊ0),��1��Ϊgb2312λ�Ŵ�1��ʼ��ϣ����0��ʼ���ʼ�1

	int index = sectionIndex * 94 + locationIndex; //gb2312ÿ��94���ַ��������֤hash�Ľ������λ����һһ��Ӧ��
	return index;
}

/*����hash����2,���gb2312������û�г��ֵĺ��ֵ�hash����
����: ����(�ָߵ�2�ֽ�)
���: �ú��ֵ�������
*/
int hashGbk(const char* ch) {
	int highIndex = (ch[0] + 256) % 256 - 0x81;
	int lowIndex;
	if ((ch[1] + 256) % 256 > 0x7f)
		lowIndex = (ch[1] + 256) % 256 - 0x40 - 1; //�ڶ��ֽڲ�����0x7f�����Եڶ��ֽڱ�0x7f����ٶ��1��������ֹhash�հ׿ռ���˷�
	else
		lowIndex = (ch[1] + 256) % 256 - 0x40;

	int index = highIndex * 190 + lowIndex;
	return index;
}
