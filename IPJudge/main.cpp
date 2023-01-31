#include <stdio.h>
#include <iostream>


#define Max 100
#define DWORD unsigned int

using namespace std;

//IP
struct Ip {
    DWORD Part[4];
};
//��������
struct SubIp {
    DWORD Part[4];
};

//·�ɱ���
typedef struct routingTableItem {
    Ip ip;          //IP��ַ
    SubIp subIP;    //��������
}tableItem;

//·�ɱ�
typedef struct routingTable {
    tableItem item[Max];    //·�ɱ�
    DWORD checksum[Max];    //У���
    int n;
}table;

Ip IP;

//��ȡһ��IP��ַ���ж��Ƿ�Ϸ�
bool getIP(int& type) {
    char c;
    printf("������һ��ip��ַ:");
    while (true) {
        fflush(stdin);
        if (5 == scanf_s("%d.%d.%d.%d%c", &IP.Part[0], &IP.Part[1], &IP.Part[2], &IP.Part[3], &c)) {
            if (0 <= IP.Part[0] && IP.Part[0] <= 255
                && 0 <= IP.Part[1] && IP.Part[1] <= 255
                && 0 <= IP.Part[2] && IP.Part[2] <= 255
                && 0 <= IP.Part[3] && IP.Part[3] <= 255
                && '\n' == c) {
                //all 0 or all 1
                if ((IP.Part[0] == 0 && IP.Part[1] == 0 && IP.Part[2] == 0 && IP.Part[3] == 0) || (IP.Part[0] == 255 && IP.Part[1] == 255 && IP.Part[2] == 255 && IP.Part[3] == 255))
                    return false;
                // private address
                if ((IP.Part[0] == 10) ||
                    (IP.Part[0] == 172 && IP.Part[1] >= 16 && IP.Part[1] <= 31) ||
                    (IP.Part[0] == 192 && IP.Part[1] == 168))
                    type = 0;
                else
                    type = 1;
                return true;
            }
            else
                return false;
        }
        else
            printf("�����ip��ַ��ʽ����!\n����������:\n");
    }
}

//ͨ����IP��ͬ�������������intֵ
DWORD intIP(Ip ip, SubIp subip) {
    DWORD ipPart[4], subipPart[4];
    for (int i = 0; i < 4; i++) {
        ipPart[i] = ip.Part[i];
        subipPart[i] = subip.Part[i];
    }
    DWORD fin = 0;
    int j = 3, k = 0, i1, i2, i3;
    //ͨ������������ļ���õ���Ӧ��intֵ
    for (int i = 0; i < 32; i++) {
        i1 = ipPart[j] % 2;//ȡ��ǰIP������
        ipPart[j] /= 2;
        i2 = subipPart[j] % 2;//ȡ�������������
        subipPart[j] /= 2;
        i3 = i1 & i2;//��λ��
        fin *= 2;//����
        fin += i3;
        if (++k == 8) {
            j--;
            k = 0;
        }
    }
    return fin;
}

//���ж��Ƿ�Ϊ�����ַ
bool NetAdress(Ip ip, table* t) {
    for (int i = 0; i < t->n; i++)
        if (ip.Part[0] == t->item[i].ip.Part[0] && ip.Part[1] == t->item[i].ip.Part[1] && ip.Part[2] == t->item[i].ip.Part[2] && ip.Part[3] == t->item[i].ip.Part[3])
            return true;
    return false;
}

//��ʼ��·�ɱ�
table* InitTable() {
    table* t;
    t = (table*)malloc(sizeof(table));
    t->n = 0;

    t->item[0].ip.Part[0] = 2;       t->item[0].ip.Part[1] = 0;       t->item[0].ip.Part[2] = 0;       t->item[0].ip.Part[3] = 0;
    t->item[0].subIP.Part[0] = 255;  t->item[0].subIP.Part[1] = 255;  t->item[0].subIP.Part[2] = 255;  t->item[0].subIP.Part[3] = 0;
    t->n++;

    t->item[1].ip.Part[0] = 2;       t->item[1].ip.Part[1] = 1;       t->item[1].ip.Part[2] = 0;       t->item[1].ip.Part[3] = 0;
    t->item[1].subIP.Part[0] = 255;  t->item[1].subIP.Part[1] = 255;  t->item[1].subIP.Part[2] = 0;    t->item[1].subIP.Part[3] = 0;
    t->n++;

    //Ĭ�����أ��������δ�������
    t->item[2].ip.Part[0] = 0;       t->item[2].ip.Part[1] = 0;       t->item[2].ip.Part[2] = 0;       t->item[2].ip.Part[3] = 0;
    t->item[2].subIP.Part[0] = 0;    t->item[2].subIP.Part[1] = 0;    t->item[2].subIP.Part[2] = 0;    t->item[2].subIP.Part[3] = 0;
    t->n++;

    for (int i = 0; i < t->n; i++)
        t->checksum[i] = intIP(t->item[i].ip, t->item[i].subIP);
    return t;
}

//�ж��Ƿ����в��������еı���
int isCatch(table* t) {
    for (int i = 0; i < t->n; i++) {
        DWORD cs = intIP(IP, t->item[i].subIP);
        if (cs == t->checksum[i])
            return i;
    }
    return -1;
}

void display(tableItem item) {
    cout << "IP��ַΪ" << item.ip.Part[0] << "." << item.ip.Part[1] << "." << item.ip.Part[2] << "." << item.ip.Part[3] << endl;
    cout << "��������Ϊ" << item.subIP.Part[0] << "." << item.subIP.Part[1] << "." << item.subIP.Part[2] << "." << item.subIP.Part[3] << endl;
    cout << endl;
}
void showTable(table* t) {
    cout << "****************\nNow the Table is :\n***************\n" << endl;
    cout << "****************" << endl;
    for (int i = 0; i < t->n; i++)
        display(t->item[i]);
    cout << "****************" << endl;
}

int main()
{
    table* T;
    T = InitTable();
    showTable(T);

    int type;
    while (true) {
        if (!getIP(type)) {
            cout << "�Ƿ���ַ��" << endl;
            break;
        }
        if (!NetAdress(IP, T)) {
            if (type)
                cout << "*���е�ַ*" << endl;
            else
                cout << "#˽�е�ַ#" << endl;
            int c = isCatch(T);
            if (c != -1) {
                cout << "catch��\n" << endl;
                display(T->item[c]);
            }
            else
                cout << "miss��" << endl;
        }
        else
            cout << "�Ƿ���ַ��(�����ַ)" << endl;
    }
    return 0;
}