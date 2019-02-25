#!/usr/bin/env python
f1 = open('myOUt.txt', 'r') 
#f2 = open('../../../DPDFsets/ZEUS_DPDF_2009SJ_NLO_pom/ZEUS_DPDF_2009SJ_NLO_pom_0000.plain', 'r') 
f2 = open('../../../outMy', 'r') 

lines1 = f1.readlines()
lines2 = f2.readlines()

def comp(n1, n2):
    return 0.5*abs(n1-n2)/(abs(n1)+abs(n2)+1e-6)


for l1, l2 in zip(lines1, lines2):
    n1 = map(float, l1.split())
    n2 = map(float, l2.split())
    
    for a1, a2 in zip(n1, n2):
        res = comp(a1, a2)
        if res > 3e-6:
            print(res)
    #break


f1.close()
