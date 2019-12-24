#include "CommandParser.h"
#include <iostream>
using namespace std;

string CommandParser::parse(string text){
    if (text.find("login")!=0 && text.find("get")!=0 && text.find("update")!=0 && text.find("logout")!=0)
        return "Comanda nu poate fi decat login, logout, get sau update.";
    if (text.find("login")==0){
        text=text.substr(5);
        if (text=="") return "Sintaxa comenzii de login este \"login username password\".";
        if (text[0]!=' ') return "Comanda nu poate fi decat login, logout, get sau update.";
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text=="") return "Sintaxa comenzii de login este \"login username password\".";
        while (text!=""&&text[0]!=' ') text=text.substr(1);
        if (text=="") return "Sintaxa comenzii de login este \"login username password\".";
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text=="") return "Sintaxa comenzii de login este \"login username password\".";
        while (text!=""&&text[0]!=' ') text=text.substr(1);
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text!="") return "Sintaxa comenzii de login este \"login username password\".";
        return "OK login";
    }
    if (text.find("get")==0){
        text=text.substr(3);
        if (text=="") return "Sintaxa comenzii de get este \"get nha\" sau \"get nhd\".";
        if (text[0]!=' ') return "Comanda nu poate fi decat login, logout, get sau update.";
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text=="") return "Sintaxa comenzii de get este \"get nha\" sau \"get nhd\".";
        if (text.find("nhd")!=0&&text.find("nha")!=0) return "Sintaxa comenzii de get este \"get nha\" sau \"get nhd\".";
        text=text.substr(3);
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text!="") return "Sintaxa comenzii de get este \"get nha\" sau \"get nhd\".";
        return "OK get";
    }
    if (text.find("update")==0){
        text=text.substr(6);
        if (text=="") return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        if (text[0]!=' ') return "Comanda nu poate fi decat login, logout, get sau update.";
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||text[0]!='.') return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||text[0]!='.') return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||text[0]!=':') return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); if (text==""||!isdigit(text[0])) return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        text=text.substr(1); while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text!="") return "Sintaxa comenzii de update este \"update zz.ll.aaaa hh:mm\", unde primul argument este o data valida, iar al doilea o ora valida.";
        return "OK update";
    }
    if (text.find("logout")==0){
        text=text.substr(6);
        while (text!=""&&text[0]==' ') text=text.substr(1);
        if (text!="") return "Comanda de logout nu are argumente!";
        return "OK logout";
    }
}