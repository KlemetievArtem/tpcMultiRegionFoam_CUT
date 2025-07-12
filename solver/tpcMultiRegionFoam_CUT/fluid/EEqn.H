   volScalarField& T_ = thermo.T();
   volScalarField q_pc_old = q_pc ;
   //scalar deltaTmid;

	
	scalar Relax = (1-Qrelax)/Qcycles;
	
	volScalarField rhoCp = rho * Cp;
	surfaceScalarField rhoCpPhi = rhoPhi * fvc::interpolate(Cp);
	volScalarField lambdaEff("lambdaEff", DT + alphat*rhoCp);
    
	fvScalarMatrix EEqn
    (
	fvm::ddt(rhoCp,T_) + fvm::div(rhoCpPhi, T_)
	- fvm::laplacian(lambdaEff, T_) 
	- fvm::Sp(fvc::ddt(rhoCp)+fvc::div(rhoCpPhi), T_)
	 ==
      - q_pc
    );

    EEqn.relax();

    EEqn.solve(mesh.solver(T_.select(finalIter)));

    if(SourceType == 0 ){ //"KRK - Kleiner, Rehfeldt, Klein model [https://github.com/ThomasKleiner/tpcMultiRegionFoam]"
	q_pc = q_pc + (Qrelax + i * Relax) * rho * Cp *((T_ - Tsat) / runTime.deltaT());
	q_pc = q_pc * neg(q_pc) * (1 - alpha1Lim) * pos(Tsat - T_) + q_pc * pos(q_pc) * alpha1Lim * neg(Tsat - T_);
    	q_pc = max(q_pc, - (1-alpha1Lim) * dhv * rho1 / runTime.deltaT()) * neg(q_pc) * pos(Tsat - T_) + min(q_pc, alpha1Lim * dhv * rho2 / runTime.deltaT()) * pos(q_pc) * neg(Tsat - T_);//additionally added line inspired from https://github.com/MahdiNabil/CFD-PC
    }
    else if(SourceType == 1 ){ //"LC - Lee model"
    	q_pc = pos(T_ - Tsat)*dhv*SourceConstant*alpha1Lim*rho1*(T_ - Tsat)/Tsat + neg(T_ - Tsat)*dhv*SourceConstant*(1 - alpha1Lim)*rho2*(T_ - Tsat)/Tsat;
    	//q_pc = max(q_pc, - (1-alpha1Lim) * dhv * rho1 / runTime.deltaT()) * neg(q_pc) * pos(Tsat - T_) + min(q_pc, alpha1Lim * dhv * rho2 / runTime.deltaT()) * pos(q_pc) * neg(Tsat - T_);
    }
    else if(SourceType == 2 ){ //"LCA - Lee model with automatic constant calculation (Minko model)"
    	q_pc = pos(T_ - Tsat)*2*DT2/(minCellLength*minCellLength)*alpha1Lim*(T_ - Tsat) + neg(T_ - Tsat)*2*DT1/(minCellLength*minCellLength)*(1 - alpha1Lim)*(T_ - Tsat);
    	//q_pc = max(q_pc, - (1-alpha1Lim) * dhv * rho1 / runTime.deltaT()) * neg(q_pc) * pos(Tsat - T_) + min(q_pc, alpha1Lim * dhv * rho2 / runTime.deltaT()) * pos(q_pc) * neg(Tsat - T_);
    }
    else if(SourceType == 3 ){ //"none"
    	q_pc = q_pc * 0;
    }


	dhe = q_pc;
  T_.correctBoundaryConditions();

   Info<< "Min/max T: " << min(thermo.T()).value() << ' '
       << max(thermo.T()).value() << endl;
