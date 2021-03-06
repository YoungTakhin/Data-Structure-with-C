# 数据结构模板

## 概述
        本模板是一本关于数据结构的参考手册。作为一本简要示例数据结构的模板，它的目的是将一些常见的数据结构进行归类、统一并呈现给读者。
	    编者认为用伪代码来描述算法相当于纸上谈兵，因此本模板通篇使用C99标准的C语言代码编写，并且尽我可能的加了一些注释来帮助读者理解
	代码，模版内的全部代码都分别在DevC++及Visual Studio 2017中测试运行通过，这些代码是开源的，读者可以直接在非正式场合中使用这些代
	码，但如果想在以盈利的目的下使用本模板中的代码请与我联系。
        源代码可以到本人的Github上下载，下载地址：
				https://github.com/YoungTakhin/Data-Structure-with-C/archive/master.zip
        因编者水平有限，模板中的bug在所难免，望读者批评指正并帮助我完善这套数据结构模板，请与我联系。

## 本模板约定
### 标题部分

	一级标题使用黑体三号字加粗；
	二级标题使用黑体四号字加粗；
	三级标题使用黑体小四号字加粗。

### 文字部分

	汉语使用宋体五号字；
	英文使用Consolas五号等宽字体。

### 代码部分

	汉语使用宋体11.5号字；
	英文使用Consolas11.5号等宽字体；
	并且为了方便读者阅读，代码部分还使用了不同的颜色对代码进行了高亮处理。

## 联系方式

	QQ：	850189787
	Github：	https://github.com/YoungTakhin

## 计划
	本模板后续计划补充如下数据结构：

		双向链表
		一些模式匹配算法
		二叉树三叉链式结构
		一些图的其他基本操作
		图的以邻接表构造
		图的关键路径算法
		线索二叉树
		B树
		红黑树

## 致谢
	在本模板的编写过程中，云南民族大学数学与计算机科学学院的王康、龙品池同学帮助我进行代码的调试和测试，在此表示衷心的感谢！

## 目录
	1.线性表 1
		1.1.顺序表	 1
			1.1.1.	定义	1
			1.1.2.	表示	2
			1.1.3.	实现	2
			1.1.4.	测试	5
		1.2.链表	7
			1.2.1.	定义	7
			1.2.2.	表示	8
			1.2.3.	实现	9
			1.2.4.	测试	12
		1.3.静态链表  14
			1.3.1.	定义	14
			1.3.2.	表示	15
			1.3.3.	实现	16
			1.3.4.	测试	19
	2．栈  22
		2.1.顺序栈	22
			2.1.1.定义	22
			2.1.2.表示	22
			2.1.3.实现	23
			2.1.4.测试	24
		2.2.两栈共享空间	25
			2.2.1.定义	25
			2.2.2.表示	26
			2.2.3.实现	26
			2.2.4.测试	28
		2.3.链栈	29
			2.3.1.定义	29
			2.3.2.表示	30
			2.3.3.实现	31
			2.3.4.测试	32
	3．队列	34
		3.1.顺序循环队列	34
			3.1.1.定义	34
			3.1.2.表示	34
			3.1.3.实现	35
			3.1.4.测试	36
		3.2.链队列	37
			3.2.1.定义	37
			3.2.2.表示	38
			3.2.3.实现	39
			3.2.4.测试	41
	4．串	43
		4.1.串	43
			4.1.1.定义	43
			4.1.2.表示	44
			4.1.3.实现	45
			4.1.4.测试	50
	5．树	52
		5.1.二叉树顺序结构	52
			5.1.1.定义	52
			5.1.2.表示	53
			5.1.3.实现	55
			5.1.4.测试	59
		5.2.二叉树二叉链式结构	61
			5.2.1.定义	61
			5.2.2.表示	62
			5.2.3.实现	65
			5.2.4.测试	70
	6.图	72
		6.1.图	72
			6.1.1.定义	72
			6.1.2.表示	73
			6.1.3.实现	77
			6.1.4.测试	87
	7.查找	89
		7.1.线性表静态查找	89
			7.1.1.表示	89
			7.1.2.实现	90
			7.1.3.测试	93
		7.2.二叉排序树（BST）动态查找	94
			7.2.1.表示	94
			7.2.2.实现	96
			7.2.3.测试	98
		7.3.平衡二叉树（AVL树）动态查找	99
			7.3.1.表示	99
			7.3.2.实现	104
			7.3.3.测试	108
		7.4.哈希表（散列表）动态查找 109
			7.4.1.表示	109
			7.4.2.实现	111
			7.4.3.测试	112
	8.排序	114
		8.1.排序	114
			8.1.1.表示	114
			8.1.2.实现	118
			8.1.3.测试	121

云南民族大学  杨德轩  2018年8月
