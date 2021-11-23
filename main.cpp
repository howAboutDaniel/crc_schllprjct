//
//  main.cpp
//  cyclicRedundancyCheck
//
//  Created by Daniel Dlugos on 18/11/2021.
//

#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <math.h>
using namespace std;

// Tento program pouziva iny pristup k zisteniu CRC ako na cviceni VTI
class CRC {
    public:
    
    string input; // Vstupna sprava (data resp. reprezentacia) v BIN
    string genPoly; // Generujuci polynom (GP)
    string divident; // Vstupna sprava rozsirena o GP
    string result; // Vysledna sprava (data + CRC)
    string crc; // CRC
    string error; // Vysledok po XOR operacii pre error_srch()
    
    int len_divident; // Dlzka spravy rozsirenej o GP
    int len_genPoly; // Dlzka GP
    int len_input; // Dlzka vstupnej spravy
    
    // Funkcia XOR
    string XOR(string a,string b)
    {
        string result = "";
        // Zistenie hodnoty prveho bitu, ak je 0, posunie sa o jednu poziciu dalej (aby sa pri XORovani nestalo ze delitel > delenec)
        if(a[0] == '0'){
            return a.substr(1);
        }
        else
        {
            for(int i = 0; i < len_genPoly; i++)
            {
                result = result + (a[i] == b[i]?'0':'1');
            }
            return result.substr(1);
        }
    }
    // Funkcia ktora deli polynom generacnym polynomom
    void delenie()
    {
        // Docasny delitel vstupnej spravuy (pretoze delitel sa v priebehu XORu meni)
        string docas_div = genPoly;
        // Docasna vstupna sprava ktora bude delena (takisto sa v jednom bode nedeli cely polynom ale iba jeho cast)
        string docas_divident = divident.substr(0,len_genPoly);
        int j = len_genPoly;
        while(j < len_divident)
        {
            docas_divident = XOR(docas_divident,docas_div);
            docas_divident = docas_divident + divident[j];
            j++;
        }
        
        crc = XOR(docas_divident,docas_div);
        result = input + XOR(docas_divident,docas_div); // Vysledna sprava
    }
    
   /*
    void get_genPoly()
    {
        int r = n - k;
        int mkIntStr = pow(10,n) + 1;
        // Prevod int na string
        stringstream ss;
        ss << mkIntStr;
        string tstr = ss.str();
        
        int mkIntStrToo = pow(10,k);
        // Prevod int na string
        stringstream tt;
        tt << mkIntStr;
        string divider = tt.str();
        
        string docas_divident = tstr.substr(0,k);
        string docas_div = divider.substr(0,k);
        
        int j = 0;
        while (j < divider.length())
        {
            docas_divident = XOR(docas_divident,docas_div);
            docas_divident = docas_divident + divider[j];
            j++;
        }
        string remain = XOR(docas_divident, docas_div);
        cout << remain << "\n";
    }
    */
    
    void getdata()
    {
        cout << "Vložte správu v BIN: ";
        cin >> input;
        
        cout << "Generačné polynómy pre váš kód: \n";
        cout << "1101\n";
        cout << "1011\n";
        cout << "Vložte jeden z vyššie uvedených: ";
        cin >> genPoly;
        
        // get_genPoly();
        
        len_genPoly = genPoly.length(); // Ziskanie dlzky GP
        len_input = input.length(); // Ziskanie dlzky vstupnej spravy
        divident  =  input;
        
        // Vstupna sprava rozsirena o 0 podla dlzky GP
        int r = len_genPoly - 1; // Zistenie poctu 0 na rozsirenie vstupnej spravy
        for(int i = 0; i < r; i++)
        {
            divident = divident + '0';
        }
        
        len_divident = divident.length(); // Ziskanie dlzky vystupnej spravy po rozsireni o GP
        delenie();
    }
    
    // Recap spravy ktora sa bude vysielat
    void transmiter()
    {
        cout << "\nVstupná správa: " << input << "\n";
        cout << "Generujúci Polynóm: " << genPoly << "\n";
        // cout << "Vstupná správa posunutá o stupeň GP : "<<divident<<"\n";
        cout << "CRC: " << crc << "\n";
        cout << "Výsledná zabezpečujúca správa: " << result << "\n";
        
    }
/*
    // Hladanie miesta s chybou
    void error_srch()
    {
        double j = len_genPoly;
        int mkIntStr = pow(10.0,j);
        // Prevod int na string
        stringstream ss;
        ss << mkIntStr;
        string str = ss.str();
        
        string docas_error_divident = str.substr(0,len_genPoly);
        string docas_error_div = genPoly;
        
        int len_mkIntStr = str.length();
        
        while (j < len_mkIntStr)
        {
            docas_error_divident = XOR(docas_error_divident,docas_error_div);
            docas_error_divident = docas_error_divident + str[j];
            j++;
        }
        error = XOR(docas_error_divident, docas_error_div);
        cout << "Výsledok po delení" << error << "\n";
        if (error == remain)
        {
            cout << "Chyba je na " << j << ". mieste\n";
        }
    }
*/
    // Dekodovanie - podobna funkcia ako delenie()
    void receiver()
    {
        string data_rec; // data_recieved
        cout << "\nVložte výslednú zabezpečujúcu správu: ";
        cin >> data_rec;

        string docas_div = genPoly;
        string docas_divident = data_rec.substr(0,len_genPoly);
        int j = len_genPoly;
        while(j < data_rec.length())
        {
            docas_divident = XOR(docas_divident,docas_div);
            docas_divident = docas_divident + data_rec[j];
            j++;
        }
        string remain = XOR(docas_divident, docas_div);
        cout << "Zvyšok po delení: \n" << remain;
        
        bool flag = 0;
        for(int i = 0; i < len_genPoly - 1; i++)
        {
            if(remain[i] == '1')
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
            cout<<"\nSpráva bola prenesená bez chyby\n";
        else
            cout<<"\nSpráva bola prenesená s chybou\n";
            // error_srch();
    }
};
 
int main()
{
     CRC crc;
     crc.getdata();
     crc.transmiter();
     crc.receiver();
     return 0;
}
