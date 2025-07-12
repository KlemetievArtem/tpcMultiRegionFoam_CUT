
//volScalarField (dXCV*dXCV)_ = 2.0e-12/(dXCV*dXCV);
//std::cout.setstate(std::ios_base::failbit);   //turn off calculation output for iteration cycles
scalar Linear = (1-Qrelax)/Qcycles;   //get portion of linear relaxation factor increase
const volScalarField alpha1Lim( max(min(alpha1, scalar(1)), scalar(0)) ); //limit alpha1 to 0 < alpha1 < 1:

volScalarField mdot = q_pc /(dhv * rho1)*runTime.deltaT();
if(turbulence.type()=="Stokes"){
	alphat = alphat*0.0;
}
else{
	alphat = (1 - alpha1Lim)*turbulence.nut()/0.85;
	alphat.correctBoundaryConditions();
}
dimensionedScalar mdot_res=1;
dimensionedScalar mdot_meanRes=1;
if(QIterationFlag==1){
	bool TLoop=true;

	  Info << nl <<">>>>>Enthalpy equation (T)" << endl;
	     Info <<"-------Energy Loop Iteration 1----------" << "Qrelax: " << Qrelax << "-----" << endl;
	while (TLoop)
	{
	  if(i==1)
	  {
	    std::cout.setstate(std::ios_base::failbit); //turn off calculation output for iteration cycles
	  }
	  if(mdot_res.value() < QResidual || i==Qcycles)
	  {
	    TLoop=false;
	    std::cout.clear();//turn on calculation output for iteration cycles

	    Info <<"-------Energy Loop Iteration " << i <<" -----" << endl;
	//      Info << nl <<"-------pos(0)= " << pos(0) << "-----" << "neg(0)= " << neg(0) << "-----" << endl;
	  }	  
	  #include "EEqn.H"	  
	  volScalarField mdotNew = q_pc /(dhv * rho1)*runTime.deltaT();
	  mdot_res = gMax(mag(mdotNew.internalField()-mdot.internalField())()); // added residuum control according to the work of Faden et al. 2017
	  mdot_meanRes = gSum(mag(mdotNew-mdot)*mesh.V())/gSum(mesh.V());
	  
	  mdot = mdotNew;
	  i+=1;
	}
}
else{
    if(SourceType == 0 ){ //"KRK - Kleiner, Rehfeldt, Klein model [https://github.com/ThomasKleiner/tpcMultiRegionFoam]"
	volScalarField& T_ = thermo.T();
	volScalarField q_pc_old = q_pc ;	
	volScalarField rhoCp = rho * Cp;
	surfaceScalarField rhoCpPhi = rhoPhi * fvc::interpolate(Cp);
	volScalarField lambdaEff("lambdaEff", DT + alphat*rhoCp);
	
	fvScalarMatrix EEqn
	(
	fvm::ddt(rhoCp,T_) + fvm::div(rhoCpPhi, T_)
	- fvm::laplacian(lambdaEff, T_) 
	- fvm::Sp(fvc::ddt(rhoCp)+fvc::div(rhoCpPhi), T_)
	 ==
	- pos(T_ - Tsat)*(fvm::SuSp(rho*Cp*alpha1Lim/runTime.deltaT(), T_) - rho*Cp*alpha1Lim/runTime.deltaT()*Tsat)
	- neg(T_ - Tsat)*(fvm::SuSp(rho*Cp*(1 - alpha1Lim)/runTime.deltaT(), T_) - rho*Cp*(1 - alpha1Lim)/runTime.deltaT()*Tsat)
	);
	q_pc = pos(T_ - Tsat)*rho*Cp*((T_ - Tsat)/runTime.deltaT())*alpha1Lim + neg(T_ - Tsat)*rho*Cp*((T_ - Tsat)/runTime.deltaT())*(1 - alpha1Lim);
	EEqn.relax();
	EEqn.solve(mesh.solver(T_.select(finalIter)));

	dhe = q_pc;
	T_.correctBoundaryConditions();

	Info<< "Min/max T: " << min(thermo.T()).value() << ' ' << max(thermo.T()).value() << endl;
	volScalarField mdotNew = q_pc /(dhv * rho1)*runTime.deltaT();	  
	mdot = mdotNew;
    }
    else if(SourceType == 1 ){ //"LC - Lee model"
	volScalarField& T_ = thermo.T();
	volScalarField q_pc_old = q_pc ;	
	volScalarField rhoCp = rho * Cp;
	surfaceScalarField rhoCpPhi = rhoPhi * fvc::interpolate(Cp);
	volScalarField lambdaEff("lambdaEff", DT + alphat*rhoCp);
	
	fvScalarMatrix EEqn
	(
	fvm::ddt(rhoCp,T_) + fvm::div(rhoCpPhi, T_)
	- fvm::laplacian(lambdaEff, T_) 
	- fvm::Sp(fvc::ddt(rhoCp)+fvc::div(rhoCpPhi), T_)
	 ==
	- pos(T_ - Tsat)*(fvm::Sp(dhv*SourceConstant*alpha1Lim*rho1/Tsat, T_) - dhv*SourceConstant*alpha1Lim*rho1/Tsat*Tsat)
	- neg(T_ - Tsat)*(fvm::Sp(dhv*SourceConstant*(1 - alpha1Lim)*rho2/Tsat, T_) - dhv*SourceConstant*(1 - alpha1Lim)*rho2/Tsat*Tsat)
	);
    	q_pc = pos(T_ - Tsat)*dhv*SourceConstant*alpha1Lim*rho1*(T_ - Tsat)/Tsat + neg(T_ - Tsat)*dhv*SourceConstant*(1 - alpha1Lim)*rho2*(T_ - Tsat)/Tsat;
	EEqn.relax();
	EEqn.solve(mesh.solver(T_.select(finalIter)));

	dhe = q_pc;
	T_.correctBoundaryConditions();

	Info<< "Min/max T: " << min(thermo.T()).value() << ' ' << max(thermo.T()).value() << endl;
	volScalarField mdotNew = q_pc /(dhv * rho1)*runTime.deltaT();	  
	mdot = mdotNew;
    }
    else if(SourceType == 2 ){ //"LCA - Lee model with automatic constant calculation (Minko model)"
	volScalarField& T_ = thermo.T();
	volScalarField q_pc_old = q_pc ;	
	volScalarField rhoCp = rho * Cp;
	surfaceScalarField rhoCpPhi = rhoPhi * fvc::interpolate(Cp);
	volScalarField lambdaEff("lambdaEff", DT + alphat*rhoCp);
	
	fvScalarMatrix EEqn
	(
	fvm::ddt(rhoCp,T_) + fvm::div(rhoCpPhi, T_)
	- fvm::laplacian(lambdaEff, T_) 
	- fvm::Sp(fvc::ddt(rhoCp)+fvc::div(rhoCpPhi), T_)
	 ==
	- pos(T_ - Tsat)*(fvm::Sp(2*DT2/(minCellLength*minCellLength)*alpha1Lim, T_) - 2*DT2/(minCellLength*minCellLength)*alpha1Lim*Tsat)
	- neg(T_ - Tsat)*(fvm::Sp(2*DT1/(minCellLength*minCellLength)*(1 - alpha1Lim), T_) - 2*DT1/(minCellLength*minCellLength)*(1 - alpha1Lim)*Tsat)
	);
    	q_pc = pos(T_ - Tsat)*2*DT2/(minCellLength*minCellLength)*alpha1Lim*(T_ - Tsat) + neg(T_ - Tsat)*2*DT1/(minCellLength*minCellLength)*(1 - alpha1Lim)*(T_ - Tsat);
	EEqn.relax();
	EEqn.solve(mesh.solver(T_.select(finalIter)));

	dhe = q_pc;
	T_.correctBoundaryConditions();

	Info<< "Min/max T: " << min(thermo.T()).value() << ' ' << max(thermo.T()).value() << endl;
	volScalarField mdotNew = q_pc /(dhv * rho1)*runTime.deltaT();	  
	mdot = mdotNew;	
    }
    else if(SourceType == 3 ){ //"none"
	volScalarField& T_ = thermo.T();
	volScalarField q_pc_old = q_pc ;	
	volScalarField rhoCp = rho * Cp;
	surfaceScalarField rhoCpPhi = rhoPhi * fvc::interpolate(Cp);

	Info<< "Min/max T: " << min(thermo.T()).value() << ' ' << max(thermo.T()).value() << endl;
	volScalarField mdotNew = q_pc /(dhv * rho1)*runTime.deltaT();	  
	mdot = mdotNew;
    }
}
Info<< "delta m residual: " << mdot_res.value() << " delta m mean residual: " << mdot_meanRes.value() << endl;

p == p_rgh + rho*gh;
