## check IP-address-by-C
By C, achieving to judge IP address, some like judge an IP adress that use an IP table to check whether legal


#一、实验概述
##1.1 实验题目
编写程序，实现判断一个IP地址是否合法，并判断该地址是否属于一个给定子网。
##1.2	思路分析
根据Ipv4的特点，采用点分十进制的形式，每部分范围为0~255，故初步进行判断。
对特殊地址进行分析，例如私有地址10.0.0.0~10.255.255.255，172.16.0.0~172.31.255.255，192.168.0.0~192.168.255.255，并根据地址范围判断为私有地址还是公有地址。
通过建立一张路由表实现子网的匹配，对每个表项通过其子网掩码与网络地址进行按位与运算通过无符号int型表示，做校验码。
每输入一个IP地址如表中的子网掩码做按位与运算，通过运算结果与表中校验和做比对，找到匹配项即为对应子网。若设置默认路由项0.0.0.0/0，则任何项均可被匹配。
 * * *
#二、设计分析
##采用结构体如下：
//IP  
struct Ip {  
3.	    DWORD Part[4];  
4.	};  
5.	//子网掩码  
6.	struct SubIp {  
7.	    DWORD Part[4];  
8.	};  
9.	  
10.	//路由表项  
11.	typedef struct routingTableItem {  
	    Ip ip;          //IP地址  
    SubIp subIP;    //子网掩码  
	}tableItem;  
	  
	//路由表  
	typedef struct routingTable {  
	    tableItem item[Max];    //路由表项  
    DWORD checksum[Max];    //校验和  
   int n;  //表项数	
}table;  
由点分十进制的特点，每个IP和子网掩码分为四个部分，对应实际IP写法。
其中DWORD为unsigned int型，32位无符号整数，符合网络地址的特性，用于存储校验和。


**本题的核心算法在于网络地址校验和的计算，即代码中intIP()及isCatch()。**
***
以下对intIP()详解：
1.	DWORD intIP(Ip ip, SubIp subip) {  
2.	    DWORD ipPart[4], subipPart[4];  //存储IP地址和子网掩码
3.	    for (int i = 0; i < 4; i++) {  
4.	        ipPart[i] = ip.Part[i];  
5.	        subipPart[i] = subip.Part[i];  
6.	    }  
7.	    DWORD fin = 0;  
8.	    int j = 3, k = 0, i1, i2, i3;  
9.	    //通过对子网掩码的计算得到相应的int值  
10.	    for (int i = 0; i < 32; i++) {  
11.	        i1 = ipPart[j] % 2;//取当前IP二进制  
12.	        ipPart[j] /= 2;  
13.	        i2 = subipPart[j] % 2;//取子网掩码二进制  
14.	        subipPart[j] /= 2;  
15.	        i3 = i1 & i2;//按位与  
16.	        fin *= 2;//左移  
17.	        fin += i3;  
18.	        if (++k == 8) {  
19.	            j--;  
20.	            k = 0;  
21.	        }  
22.	    }  
23.	    return fin;  
24.	}
***
1.	//判断是否命中并返回命中的表项  
2.	int isCatch(table* t) {  
3.	    for (int i = 0; i < t->n; i++) {  
4.	        DWORD cs = intIP(IP, t->item[i].subIP);  
5.	        if (cs == t->checksum[i])  
6.	            return i;  
7.	    }  
8.	    return -1;  
9.	}  

**通过二进制运算特点，每次对数据取模即取其二进制对应位，同子网掩码的对应二进制位相与，通过每次乘2实现算数左移，最终计算结果为对应校验和，并与对应子网校验和进行必对，若相等则命中，若不等则继续对其他表项运算，因为默认路由的存在，一定会有一项相匹配。**
 
