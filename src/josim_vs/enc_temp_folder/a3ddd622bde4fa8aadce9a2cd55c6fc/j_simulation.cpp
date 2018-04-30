#include "include/j_simulation.hpp"

trans_sim tsim;

void identify_simulation(InputFile& iFile) {
	std::string simline;
	for (auto i : iFile.controlPart) {
		if (i.find(".TRAN") != std::string::npos) {
			simline = i;
			iFile.simulationType = TRANSIENT;
			break;
		}
		if (i.find(".DC") != std::string::npos) {
			simline = i;
			iFile.simulationType = DC;
			break;
		}
		if (i.find(".AC") != std::string::npos) {
			simline = i;
			iFile.simulationType = AC;
		}
		if (i.find(".PHASE") != std::string::npos) {
			simline = i;
			iFile.simulationType = PHASE;
		}
	}

	std::vector<std::string> simtokens;
	switch (iFile.simulationType) {
	case TRANSIENT:
		simtokens = tokenize_delimeter(simline, " ,");
		if (simtokens.size() < 3) {
			control_errors(TRANS_ERROR, "Too few parameters");
		}
		else if (simtokens.size() >= 3) {
			tsim.prstep = modifier(simtokens[1]);
			tsim.tstop = modifier(simtokens[2]);
			if (simtokens.size() == 4) {
				tsim.tstart = modifier(simtokens[3]);
			}
			if (simtokens.size() == 5) {
				tsim.maxtstep = modifier(simtokens[4]);
			}
		}
		break;
	case DC:
	case AC:
	case PHASE:
	case NONE:
		control_errors(NO_SIM, "");
	}
}
/*
Perform transient simulation
*/
/* Where to store the calulated values */
std::vector<std::vector<double>> x;
std::vector<double> timeAxis, inductanceVector;
std::map<std::string, std::vector<double>> junctionCurrents;
void transient_simulation() {
	/* Standard vector */
	std::vector<double> lhsValues(Nsize, 0.0);
	for (int m = 0; m < Nsize; m++) {
		x.push_back(std::vector<double>(tsim.simsize(), 0.0));
	}
	/* Perform time loop */
	std::vector<double> RHS(columnNames.size(), 0.0), LHS_PRE;
	/* Variables to be used by the RHS matrix construction routine */
	std::string currentLabel, columnIndexLabel;
	//std::map<std::string, double>& currentConductance = NULL;
	std::vector<std::string> tokens;
	std::map<std::string, rcsj_sim_object> simJunctions;
	double VP, VN, CUR, LCUR, VB, RHSvalue, inductance;
	double hn_2_2e_hbar = (tsim.maxtstep / 2)*(2 * M_PI / PHI_ZERO);
	int counter, ok, columnIndex, rowCounter;
	klu_symbolic * Symbolic;
	klu_common Common;
	klu_numeric * Numeric;
	/* Set the default settings for KLU */
	ok = klu_defaults(&Common);
	/* Do symbolic analysis of matrix */
	Symbolic = klu_analyze(Nsize, &rowptr.front(), &colind.front(), &Common);
	/* Do numeric factorization of matrix */
	Numeric = klu_factor(&rowptr.front(), &colind.front(), &nzval.front(), Symbolic, &Common);
	/* Set up the junctions */
	for (auto j : rowNames) {
		if (j[2] == 'B') {
			/* Identify the junction label */
			currentLabel = substring_after(j, "R_");
			columnIndexLabel = "C_P" + substring_after(j, "R_");
			columnIndex = index_of(columnNames, columnIndexLabel);
			simJunctions[j].label = currentLabel;
			/* Try to identify the column index of the positive node */
			simJunctions[j].vPositive = (int)bMatrixConductanceMap[j].at(currentLabel + "-VP"); 
			/* Try to identifiy the column index of the negative node */
			simJunctions[j].vNegative = (int)bMatrixConductanceMap[j].at(currentLabel + "-VN"); 
			/* Try to identify the column index of the phase node, panick if not found */
			try { simJunctions[j].bPhase = (int)bMatrixConductanceMap[j].at(currentLabel + "-PHASE"); }
			catch (std::out_of_range) { simulation_errors(JJPHASE_NODE_NOT_FOUND, currentLabel); }
			/* Try to identify the junction capacitance, panick if not found */
			try { simJunctions[j].jjCap = bMatrixConductanceMap[j].at(currentLabel + "-CAP"); }
			catch (std::out_of_range) { simulation_errors(JJCAP_NOT_FOUND, currentLabel); }
			/* Try to identify the junction critical current, panick if not found */
			try { simJunctions[j].jjIcrit = bMatrixConductanceMap[j].at(currentLabel + "-ICRIT"); }
			catch (std::out_of_range) { simulation_errors(JJICRIT_NOT_FOUND, currentLabel); }
			/* If the junction positive node is connected to ground */
			if (simJunctions[j].vPositive == -1) {
				simJunctions[j].VB = -lhsValues[simJunctions[j].vNegative];
				simJunctions[j].negativeNodeRow = rowNames[simJunctions[j].vNegative];
			}
			/* If the junction negativie node is connected to ground */
			else if (simJunctions[j].vNegative == -1) {
				simJunctions[j].VB = lhsValues[simJunctions[j].vPositive];
				simJunctions[j].positiveNodeRow = rowNames[simJunctions[j].vPositive];
			}
			/* If both nodes are not connected to ground */
			else {
				simJunctions[j].VB = lhsValues[simJunctions[j].vPositive] - lhsValues[simJunctions[j].vNegative];
				simJunctions[j].positiveNodeRow = rowNames[simJunctions[j].vPositive];
				simJunctions[j].negativeNodeRow = rowNames[simJunctions[j].vNegative];
			}
		}
		else if (j[2] == 'L') {
			inductanceVector.push_back(inductanceMap[j]);
		}
	}
	/***************/
	/** TIME LOOP **/
	/***************/
	for (int i = 0; i < tsim.simsize() - 1; i++) {
		/* Start of initialization of the B matrix */
		RHS.clear();
		rowCounter = 0;
		/* Construct RHS matrix */
		for (auto j : rowNames) {
			RHSvalue = 0.0;
			//try { bMatrixConductanceMap.at(j) = bMatrixConductanceMap.at(j); }
			//catch (std::out_of_range) { }
			/* If this row item is identified as a node row then...*/
			if (j[2] == 'N') {
				tokens.clear();
				//columnIndexLabel = "C_NV" + substring_after(j, "R_N");
				//columnIndex = index_of(columnNames, columnIndexLabel);
				for (auto k : bMatrixConductanceMap.at(j)) {
					/* If this row item contains a junction add it to the list of row items */
					if (k.first[0] == 'B') {
						currentLabel = substring_before(k.first, "-");
						unique_push(tokens, currentLabel);
					}
					/* If this row item contains a current source add it to the list of row items */
					else if (k.first[0] == 'I') {
						currentLabel = k.first;
						unique_push(tokens, currentLabel);
					}
					/* If this row item contains a resistor add it to the list of row items */
					else if (k.first[0] == 'R') {
						currentLabel = substring_before(k.first, "-");
						unique_push(tokens, currentLabel);
					}
					/* If this row item contains a capacitor add it to the list of row items */
					else if (k.first[0] == 'C') {
						currentLabel = substring_before(k.first, "-");
						unique_push(tokens, currentLabel);
					}
					/* If this row item contains a inductor add it to the list of row items */
					else if (k.first[0] == 'L') {
						currentLabel = substring_before(k.first, "-");
						unique_push(tokens, currentLabel);
					}
				}
				for (auto k : tokens) {
					/* Add junction as calculated at the end of the current loop to the RHS */
					if (k[0] == 'B') {
						if(j == simJunctions["R_" + k].positiveNodeRow) RHSvalue += simJunctions["R_" + k].Is;
						else if (j == simJunctions["R_" + k].negativeNodeRow) RHSvalue -= simJunctions["R_" + k].Is;
					}
					/* Add the current value to the RHS in the correct row */
					else if (k[0] == 'I') {
						RHSvalue += sources[k][i];
					}
				}
			}
			/* If this row item is identified as an inductor row */
			else if (j[2] == 'L') {
				/* Identify the inductor label */
				currentLabel = substring_after(j, "R_");
				//columnIndexLabel = "C_I" + substring_after(j, "R_");
				//columnIndex = index_of(columnNames, columnIndexLabel);
				/* Identify the relevant inductance of the inductor */
				inductance = bMatrixConductanceMap.at(j)[currentLabel];
				/* Identify the column index of the positive node */
				VP = bMatrixConductanceMap.at(j).at(currentLabel + "-VP");
				/* Identifiy the column index of the negative node */
				VN = bMatrixConductanceMap.at(j).at(currentLabel + "-VN");
				/* Try to identifiy the column index of the inductor current node */
				try { 
					CUR = bMatrixConductanceMap.at(j).at(currentLabel + "-I");
					LCUR = lhsValues[(int)CUR];
				}
				catch (std::out_of_range) { simulation_errors(INDUCTOR_CURRENT_NOT_FOUND, currentLabel); }
				/* If the inductor positive node is connected to ground */
				if (VP == -1.0) VB = -lhsValues[(int)VN];
				/* If the inductor negative node is connected to ground */
				else if (VN == -1.0) VB = lhsValues[(int)VP];
				/* If both nodes are not connected to ground */
				else VB = lhsValues[(int)VP] - lhsValues[(int)VN];
				/* R_L = (-2L/hn)IL - VL*/
				RHSvalue = (-2 * inductance / tsim.maxtstep)*LCUR - VB;
			}
			/* If this row item is identified as a junction row */
			else if (j[2] == 'B') {
				/* Identify the junction label */
				currentLabel = substring_after(j, "R_");
				//columnIndexLabel = "C_P" + substring_after(j, "R_");
				//columnIndex = index_of(columnNames, columnIndexLabel);
				/* R_B = Phi(n-1) + (hn/2)(2e/hbar)VB */
				RHSvalue = simJunctions[j].Phase_Prev + ((hn_2_2e_hbar)*simJunctions[j].VB);
			}
			/* If this row item is identified as a voltage source row */
			else if (j[2] == 'V') {
				/* Identify the voltage source label */
				currentLabel = substring_after(j, "R_");
				//columnIndexLabel = "C_" + substring_after(j, "R_");
				//columnIndex = index_of(columnNames, columnIndexLabel);
				/* Assign the voltage source value at the current point in the time loop to the RHS value */
				RHSvalue = sources[currentLabel][i];
			}
			/* Add the RHS value as determined above to the correct spot in the RHS vector */
			//RHS[columnIndex] = RHSvalue;
			RHS.push_back(RHSvalue);
			rowCounter++;
		}
		/* End of the B matrix initialization */

		/* Solve Ax=b */
		LHS_PRE = RHS;
		ok = klu_tsolve(Symbolic, Numeric, Nsize, 1, &LHS_PRE.front(), &Common);

		/* Set the LHS values equal to the returning value provided by the KLU solution */
		lhsValues = LHS_PRE;
		for (int m = 0; m < lhsValues.size(); m++) {
			x[m][i] = lhsValues[m];
		}

		/* Guess next junction voltage */
		for (auto j : simJunctions) {
			rcsj_sim_object thisJunction = simJunctions[j.first];
			if (j.second.vPositive == -1) thisJunction.VB = (-lhsValues[j.second.vNegative]);
			else if (j.second.vNegative == -1) thisJunction.VB = (lhsValues[j.second.vPositive]);
			else thisJunction.VB = (lhsValues[j.second.vPositive] - lhsValues[j.second.vNegative]);
			thisJunction.Phase = lhsValues[j.second.bPhase];
			thisJunction.VB_dt = (2 / tsim.maxtstep)*(thisJunction.VB - thisJunction.VB_Prev) - thisJunction.VB_dt_Prev;
			thisJunction.VB_Guess = thisJunction.VB + tsim.maxtstep*thisJunction.VB_dt;
			thisJunction.Phase_Guess = thisJunction.Phase + (hn_2_2e_hbar)*(thisJunction.VB + thisJunction.VB_Guess);
			thisJunction.Is = -thisJunction.jjIcrit * sin(thisJunction.Phase_Guess) + (((2 * thisJunction.jjCap) / tsim.maxtstep)*thisJunction.VB) + (thisJunction.jjCap * thisJunction.VB_dt);
			thisJunction.VB_Prev = thisJunction.VB;
			thisJunction.VB_dt_Prev = thisJunction.VB_dt;
			thisJunction.Phase_Prev = thisJunction.Phase;
			simJunctions[j.first] = thisJunction;
			/* Store the junction currents for printing */
			junctionCurrents[j.first].push_back(thisJunction.Is);
		}
		/* Add the current time value to the time axis for plotting purposes */
		timeAxis.push_back(i*tsim.maxtstep);
	}
}