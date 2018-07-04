* gnetlist.exe -g spice-sdb -O jsim -O param -O sort_mode -v -o ./js/jj_characteristics.js C:\Users\Joey Delport\OneDrive\JD_no_Mirai\gEDA_Workspace\jj_characteristics.sch
*********************************************************
* Spice file generated by gnetlist                      *
* spice-sdb version 4.28.2007 by SDB --                 *
* provides advanced spice netlisting capability.        *
* Documentation at http://www.brorson.com/gEDA/SPICE/   *
*********************************************************
*==============  Begin SPICE netlist of main design ============
B1         1          0          jj1        area=1
IS         0          1          pwl(0      0 200p 600u 800p 600u 1000p 0)
*R1         1          2          1k        
.model jj1 jj(rtype=1, vg=2.8mV, cap=0.07pF, r0=160, rn=16, icrit=0.1mA)
.tran 1p 1000p 0 0.25p
.print DEVV B1
.print DEVI IS
*.print DEVI B1
.end
