# Example Usage

## Basic JTL Example

In this section we provide example usage of the simulator. In the *test* subfolder of the repository the user wil find multiple example files that can be used with the simulator.

In this example we will make use of the *ex_jtl_basic.cir* file. This is a basic Josephson transmission line (JTL) that simply carries the single flux quantum pulse from input to output.

~~~
* Example JTL Basic
B01        3          7          jmod     area=2.16
B02        6          8          jmod     area=2.16
IB01       0          1          pwl(0      0 5p 280u)
L01        4          3          2p        
L02        3          2          2.425p    
L03        2          6          2.425p    
L04        6          5          2.031p    
LP01       0          7          0.086p    
LP02       0          8          0.096p    
LPR01      2          1          0.278p    
LRB01      7          9          0.086p    
LRB02      8          10         0.086p    
RB01       9          3          5.23      
RB02       10         6          5.23      
ROUT       5          0          2         
VIN        4          0          pwl(0 0 300p 0 302.5p 827.13u 305p 0 600p 0 602.5p 827.13u 605p 0)
.model jmod jj(rtype=1, vg=2.8mV, cap=0.07pF, r0=160, rN=16, icrit=0.1mA)
.tran 0.25p 1000p 0 0.25p
.print DEVV VIN
.print DEVI ROUT
.print PHASE B01
.print PHASE B02
.end
~~~

<table>
  <th>Label</th><th>+ Node</th><th>- Node</th><th>Value</th>
  <tr>R</tr>
  <tr>L</tr>
  <tr>C</tr>
  <tr>B</tr>
  <tr>M</tr>
  <tr></tr>
</table>

Only the device voltage (DEVV) of the input source (VIN), the device current (DEVI) of the output resistor (ROUT) and the phase of both junctions are requested to be store.

The simulation executes for a total of 1000ps and produces a result every 0.25ps which results in 4000 points of data for each requested variable.

We simulate this circuit (on a CentOS 7 machine) using the command:

**./josim-cli -o ex_jtl_basic.csv ex_jtl_basic.cir**

This simulation is almost instant, given the size of the problem and should take no longer than 10ms to complete.

<figure>
	<img src="../img/josim_jtl_ex.png" alt="JoSIM JTL example" class="center">
	<figcaption align="center"> Fig. 1 - JoSIM JTL example commandline output.</figcaption>
</figure>

The results of this simulation are stored in the *ex_jtl_basic.csv* file of which an excerpt of the two events is shown below.

```bash
time,CURRENT_ROUT,DEVV_VIN,PHASE_B01,PHASE_B02
0.0000000000000000e+00,0.0000000000000000e+00,0.0000000000000000e+00,0.0000000000000000e+00,0.0000000000000000e+00
2.4999999999999999e-13,5.2618741532587213e-07,0.0000000000000000e+00,1.7590417807527424e-03,1.7625663594260056e-03
		.						.						.						.						.
		.						.						.						.						.
		.						.						.						.						.
2.9950000000000000e-10,1.2602334650362215e-22,0.0000000000000000e+00,4.0463239580396271e-01,6.1557778681614406e-01
2.9974999999999997e-10,-8.3663517212216735e-22,0.0000000000000000e+00,4.0463239580396271e-01,6.1557778681614406e-01
3.0000000000000000e-10,-1.5966746163438487e-21,0.0000000000000000e+00,4.0463239580396271e-01,6.1557778681614406e-01
3.0024999999999997e-10,1.2138210460568509e-08,8.2712999999999992e-05,4.0588849169065189e-01,6.1561844609608551e-01
3.0050000000000000e-10,8.3269106098336399e-08,1.6542599999999998e-04,4.1373721572716365e-01,6.1593850563537267e-01
3.0074999999999997e-10,3.1874311596945211e-07,2.4813899999999998e-04,4.3783030030526732e-01,6.1722434414431326e-01
3.0099999999999999e-10,8.8716300244636592e-07,3.3085199999999997e-04,4.8814712164259011e-01,6.2078477555864586e-01
3.0125000000000002e-10,1.9855061368009524e-06,4.1356499999999996e-04,5.7113817950274370e-01,6.2846436865880417e-01
3.0149999999999999e-10,3.7775100494815062e-06,4.9627799999999995e-04,6.8883988431911958e-01,6.4225058535926782e-01
3.0175000000000002e-10,6.3359283607783743e-06,5.7899099999999995e-04,8.4054979432033217e-01,6.6376618522513120e-01
3.0199999999999999e-10,9.6268557444681243e-06,6.6170399999999994e-04,1.0258326634823722e+00,6.9395688283705226e-01
3.0225000000000002e-10,1.3552874679952160e-05,7.4441699999999993e-04,1.2475669537636640e+00,7.3318675289877544e-01
3.0249999999999999e-10,1.8042425566922425e-05,8.2712999999999992e-04,1.5144772973301901e+00,7.8175014313659197e-01
3.0275000000000001e-10,2.3132972521939297e-05,7.4441699999999993e-04,1.8411678233070754e+00,8.4057341009994713e-01
3.0299999999999999e-10,2.9026125783186692e-05,6.6170399999999994e-04,2.2453378998772728e+00,9.1179088131163233e-01
3.0325000000000001e-10,3.6115317782987027e-05,5.7899099999999995e-04,2.7485987257643534e+00,9.9913706309331773e-01
3.0349999999999999e-10,4.5004018307525714e-05,4.9627799999999995e-04,3.3796783185915840e+00,1.1083683692206756e+00
3.0375000000000001e-10,5.6484467501108440e-05,4.1356499999999996e-04,4.1592025708753528e+00,1.2477159562171136e+00
3.0399999999999998e-10,7.1294936682528957e-05,3.3085199999999997e-04,5.0434512532682056e+00,1.4274416884444263e+00
3.0425000000000001e-10,8.9601267490561153e-05,2.4813899999999998e-04,5.8716978821424979e+00,1.6569226427300268e+00
3.0449999999999998e-10,1.1064201526112140e-04,1.6542599999999998e-04,6.4323756562633472e+00,1.9399189619742534e+00
3.0475000000000001e-10,1.3305044637426826e-04,8.2712999999999992e-05,6.6189208102948909e+00,2.2725986145252426e+00
3.0499999999999998e-10,1.5575130808877477e-04,0.0000000000000000e+00,6.4943549761561137e+00,2.6479927374623258e+00
3.0525000000000001e-10,1.7885172860297336e-04,0.0000000000000000e+00,6.2272567013493418e+00,3.0641714808700860e+00
3.0549999999999998e-10,2.0379715833211554e-04,0.0000000000000000e+00,5.9948045018496323e+00,3.5290571354520934e+00
		.						.						.						.						.
		.						.						.						.						.
		.						.						.						.						.
5.9949999999999995e-10,4.3212858202495744e-21,0.0000000000000000e+00,6.6878072510727549e+00,6.8987600821376667e+00
5.9975000000000002e-10,4.3137521994626109e-21,0.0000000000000000e+00,6.6878072510727549e+00,6.8987600821376667e+00
6.0000000000000000e-10,4.3078931225786343e-21,0.0000000000000000e+00,6.6878072510727549e+00,6.8987600821376667e+00
6.0024999999999997e-10,1.2138210460577924e-08,8.2712999999999992e-05,6.6890633469594443e+00,6.8988007414176078e+00
6.0049999999999994e-10,8.3269096666537856e-08,1.6542599999999998e-04,6.6969120695472864e+00,6.8991208009145337e+00
6.0075000000000002e-10,3.1874302598962546e-07,2.4813899999999998e-04,6.7210051436437208e+00,6.9004066390118268e+00
6.0099999999999999e-10,8.8716255780263208e-07,3.3085199999999997e-04,6.7713219264319413e+00,6.9039670683967049e+00
6.0124999999999997e-10,1.9855046057915069e-06,4.1356499999999996e-04,6.8543128850350286e+00,6.9116466546674440e+00
6.0149999999999994e-10,3.7775059001042050e-06,4.9627799999999995e-04,6.9720143838434145e+00,6.9254328535762175e+00
6.0175000000000002e-10,6.3359188963277667e-06,5.7899099999999995e-04,7.1237239212106864e+00,6.9469484148633018e+00
6.0199999999999999e-10,9.6268367676180748e-06,6.6170399999999994e-04,7.3090061730293270e+00,6.9771390393488719e+00
6.0224999999999996e-10,1.3552840137866662e-05,7.4441699999999993e-04,7.5307394915059733e+00,7.0163687836318234e+00
6.0250000000000004e-10,1.8042366960989605e-05,8.2712999999999992e-04,7.7976483370788721e+00,7.0649319712733414e+00
6.0275000000000001e-10,2.3132877643123393e-05,7.4441699999999993e-04,8.1243365485310051e+00,7.1237549237784270e+00
6.0299999999999999e-10,2.9025976163591970e-05,6.6170399999999994e-04,8.5285030132858992e+00,7.1949719126817717e+00
6.0324999999999996e-10,3.6115084492683834e-05,5.7899099999999995e-04,9.0317582599489015e+00,7.2823173512317032e+00
6.0350000000000004e-10,4.5003657487430368e-05,4.9627799999999995e-04,9.6628298406009119e+00,7.3915475072422705e+00
6.0375000000000001e-10,5.6483922613042281e-05,4.1356499999999996e-04,1.0442344928918883e+01,7.5308933521128347e+00
6.0399999999999998e-10,7.1294158923435432e-05,3.3085199999999997e-04,1.1326588459213040e+01,7.7106166231913225e+00
6.0424999999999996e-10,8.9600246674970675e-05,2.4813899999999998e-04,1.2154840250727050e+01,7.9400944882309208e+00
6.0450000000000003e-10,1.1064078717562771e-04,1.6542599999999998e-04,1.2715533169224782e+01,8.2230874188094543e+00
6.0475000000000001e-10,1.3304905904021097e-04,8.2712999999999992e-05,1.2902095586396108e+01,8.5557636754469328e+00
6.0499999999999998e-10,1.5574976667867628e-04,0.0000000000000000e+00,1.2777541168233270e+01,8.9311543029884017e+00
6.0524999999999995e-10,1.7884995879749259e-04,0.0000000000000000e+00,1.2510445228968999e+01,9.3473289350035067e+00
6.0550000000000003e-10,2.0379503512670802e-04,0.0000000000000000e+00,1.2277987682825806e+01,9.8122093480792216e+00
		.						.						.						.						.
		.						.						.						.						.
		.						.						.						.						.
9.9925000000000004e-10,9.9776412773980053e-20,0.0000000000000000e+00,1.2970982106365490e+01,1.3181942377470081e+01
9.9949999999999991e-10,9.8740906314363196e-20,0.0000000000000000e+00,1.2970982106365490e+01,1.3181942377470081e+01
9.9974999999999999e-10,9.7932385381164065e-20,0.0000000000000000e+00,1.2970982106365490e+01,1.3181942377470081e+01

```

When this output result is plotted using the Python script mentioned in [Technical Discussion](tech_disc.md) the user will be presented with a visual representation of the result.

<figure>
	<img src="../img/ex_jtl_result.pdf" alt="JoSIM JTL results" class="center">
	<figcaption align="center"> Fig. 2 - JoSIM JTL example result output.</figcaption>
</figure>

Through the reperesentation of the results visually, the user can see the two SFQ pulses at the input and through the output resistor, as well as, the phases of the two junctions.

The same process can be done for every example found in the *test* folder on the repository.

## Commandline Switches

The command line interface has various switches which can be shown using the **-h** switch. Each of these switches will also be shown and discussed below.

<table>
  <tr>
    <th>Switch</th><th>Options</th><th>Explanation</th>
  </tr>
  <tr>
    <td>-a</td><td>0 or 1</td><td rowspan="2">Sets the analysis type between Voltage and Phase mode</td>
  </tr>
  <tr>
    <td>--analysis=</td><td>0 is default</td>
  </tr>
  <tr>
    <td>-c</td><td>0 or 1</td><td rowspan="2">Sets the subcircuit convention used internally. 0 being JSIM. 1 being WRspice (standard SPICE)</td>
  </tr>
  <tr>
  	<td>--convention=</td><td>0 is default</td>
  </tr>
  <tr>
    <td>-h</td><td></td><td rowspan="2">Displays the help menu that contains explanations for each of these switches.</td>
  </tr>
  <tr>
    <td>--help</td><td></td>
  </tr>
  <tr>
    <td>-i</td><td></td><td rowspan="2">Input circuit netlist from standard input.</td>
  </tr>
  <tr>
    <td>--input</td><td></td>
  </tr>
  <tr>
    <td>-o</td><td>output filename</td><td rowspan="2">Save the output to file as either .csv, .dat or raw</td>
  </tr>
  <tr>
    <td>--output=</td><td>output.csv</td>
  </tr>
  <tr>
    <td>-p</td><td></td><td rowspan="2">Enables an experimental parallel processing feature. This is still under construction and results might not be as intendend</td>
  </tr>
  <tr>
    <td>--parallel</td><td></td>
  </tr>
  <tr>
    <td>-V</td><td></td><td rowspan="2">Enables verbose mode, displaying extra information with regards to the simulation</td>
  </tr>
  <tr>
    <td>--verbose</td><td></td>
  </tr>
  <tr>
    <td>-v</td><td></td><td rowspan="2">Displays the version information for this version of the executable</td>
  </tr>
  <tr>
    <td>--version</td><td></td>
  </tr>
</table>

These switches can be fed to the executable in any order as long as it makes sense. With this being said, the simulator will treat the first non switch related string line as the input file. It would make perfect sense to then have a command line as follows:

**./josim-cli -a 1 inputfile —output=outputfile.csv -c 1**

As this will see analysis as *1*, convention as *1* and the output file as *outputfile.csv*. This leaves the only non switch related string being *inputfile*. 

It would, however, not make sense to have convention or analysis set to a value that is not a valid option or omitting the option for the switch completely. An example would be if the command is given as:

**./josim-cli -a inputfile -c**

This would not work and would tell the user that command switches are invalid.

Additionally, when the version command is sent through the program only prints the version and then exits. This is similar to the help command, which also only prints the menu and exits.

<style>
.center {
    display: block;
    margin: 0 auto;
}
</style>