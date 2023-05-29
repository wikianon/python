#!/usr/bin/env python
#coding: utf-8
#você pode modificar o codigo e distribuir a vontade, desde quee
#nao retire o nome do Autor
#Autor: Luis Eduardo Boiko Ferreira
#e-mail: luiseduardo.boiko@gmail.com
import time
import datetime 
print ' _______________________________________'
print '|                                       |'
print '|                                       |'
print '|       Algoritmo para calcular         |'
print '|          o dia da Pascoa              |'
print '|          Desenvolvido por             |'
print '|      Luis Eduardo Boiko Ferreira      |'
print '|_______________________________________|'
anoatual = datetime.date.today().year 
print 'O ano atual é %s.' %anoatual
ano=input('Digite o ano desejado para calcularmos o dia da páscoa: ')
a=int(ano%19)
b=int(ano/100)
c=int(ano%100)
d=int(b/4)
e=int(b%4)
f=int((b+8)/25)
g=int((b-f+1)/3)
h=((19*a+b-d-g+15)%30)
i=int(c/4)
k=int(c%4)
L=((32+2*e+2*i-h-k)%7)
m=int((a+11*h+22*L)/451)
mes=int((h+L-7*m+114)/31)
if mes==1 : mes='Janeiro'
elif mes==2 : mes='Fevereiro'
elif mes==3 : mes='Março'
elif mes==4 : mes='Abril'
elif mes==5 : mes='Maio'
elif mes==6 : mes='Junho'
elif mes==7 : mes='Julho'
elif mes ==8 : mes ='Agosto'
elif mes ==9 : mes ='Setembro'
elif mes ==10 : mes ='Outubro'
elif mes ==11 : mes ='Novembro'
else : mes ='Dezembro'
mes1=mes
dia=(((h+L-7*m+114)%31)+1)
if anoatual>ano :
  print "A pascoa caiu no dia: %s."% dia 
  print "Do mês: %s" %mes1

else :
   print "A pascoa ira cair no dia: %s."% dia 
   print "Do mês: %s" %mes1 
