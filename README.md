# (IN PROCESS)

# tpcMultiRegionFoam_CUT &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;
tpcMultiRegionFoamCUT solver is modification of [tpcMultiRegionFoam](https://github.com/ThomasKleiner/tpcMultiRegionFoam) solver with:
 - Different condensation models
 - Turbulence modeling
 - Post-processing functions
 - Various improvements and fixes

tpcMultiRegionFoamCUT is CFD Solver for heat transfer simulations between solid and fluid regions with implemented models for thermal phase change of pure substances in fluid regions (Lee model - LC; Kleiner, Rehfeldt, Klein model - KRK; Lee model with AUTOMATIC constant calculation (Minko model) - LCA), turbulence model in vapor phase and calculation of wallHeatFluxes on patches. 

 ![itf](https://github.com/user-attachments/assets/2e6c9155-2388-4bb3-9853-0c021b6443c3)

[National Research University "Moscow Power Engineering Institute"](https://mpei.ru/lang/en/Pages/default.aspx), [Dept. of Engineering Thermophysics named after Vladimir Kirillin​](https://thermophys.ru/)


        © 2025 Artem Klementiev, Konstantin Minko


## Description
Curent OpenFOAM solver is modification of CFD solver [tpcMultiRegionFoam](https://github.com/ThomasKleiner/tpcMultiRegionFoam), first presented and validated in the article [Kleiner 2019](https://www.sciencedirect.com/science/article/pii/S0017931018353055).
CFD solver was further modified with different condnsation models (models for thermal phase change of pure substances).
# Condensation Models
1. LC: first published in [Lee 1980](https://www.scopus.com/pages/publications/84876465720);
2. KRK: first published in [Kleiner 2019](https://www.sciencedirect.com/science/article/pii/S0017931018353055);
3. LCA: first published in [Minko, Artemov, Klementiev 2022](https://link.springer.com/article/10.1134/S0040601523030059).
Linearization of (condensation) source term in energy conservation equation is performed.
# Turbulence Model
Model from [Da Riva,2009](https://asmedigitalcollection.asme.org/MNHT/proceedings/MNHMT2009/43901/139/334977) for dampenning turbulence in condensate phase is implemented.
# Validation and Verification
 - Stefan proplem
 - vapor condensation on vertical plane
 - condensation of Stagnant or Moving vapor on horizontal cylinder
 - cross-verification of LCA condenstion model using OpenFOAM, Ansys Fluent and ANES perfermed in [Minko, Artemov, Klementiev 2025](). 

# Citing This Work
If using the LCA condensation model or concepts from this repository leads to publication, please cite: [Minko, Artemov, Klementiev 2022](https://link.springer.com/article/10.1134/S0040601523030059) 
Further refinement of the solver is welcome (especially turbulence model)!

## Installation

🟡
For installation, the [OpenFOAM 6 libraries](https://github.com/OpenFOAM/OpenFOAM-6) have to be loaded. 

All necessary applications for the solver installation can be installed and uninstalled by the supplied [Allwmake](https://github.com/ThomasKleiner/tpcMultiRegionFoam/tree/master/solver/Allwmake) and [Allwclean](https://github.com/ThomasKleiner/tpcMultiRegionFoam/tree/master/solver/Allwclean) shell scripts, respectively. The scripts are located in the [solver](https://github.com/ThomasKleiner/tpcMultiRegionFoam/tree/master/solver) folder.

Copy the content of the solver directory in your desired user applications folder and run the [Allwmake](https://github.com/ThomasKleiner/tpcMultiRegionFoam/tree/master/solver/Allwmake) script.
```
Working folder:/home/<user>/OpenFOAM/<user>-6/applications
        
$ cd /home/<user>/OpenFOAM/<user>-6/applications
$ cd solver
$ chmod 700 Allw*
$ ./Allwmake
```
🟡

## Provided Test Cases

🟡
Two test cases, a 2D and a 3D test case, are provided for a transient simulation of pure substance condensation on a horizontal tube. 
Two regions are considered in both simulations. One solid region for the tube and one fluid region for the two phase system. Both Regions are thermally coupled at the outer tube wall. A constant heat flux is withdrawn from the inner wall of the Tube. The initial state of the simulation consists of a pure vapor phase at saturation conditions with a temperature of 309.15 K (*n*-pentane, 1.013 bar). The initial tube temperature is also set to 309.15 K.

In the first couple of time steps, the tube is cooling down and no condensate is formed until the temperature change reaches the outer tube wall. Condensate is formed, as soon as a temperature decrease below saturation conditions would occur in the fluid region.  
Since a very thin condensate film is formed within the simulations, high computational effort is necessary for a very good quantitative agreement with measurements. Since we aim to give an insight in the simulation set up with the provided test cases, they have a lower mesh resolution and a higher time step size compared to the simulations published in  ([Kleiner 2019](https://www.sciencedirect.com/science/article/pii/S0017931018353055)). 
🟡

### 2D-Simulation of pure substance film condensation on a horizontal smooth tube
The presented results of the 2D simulation shows the tube’s temperature field in the rainbow scale and the fluid’s volume phase fraction field in the grey scale. 

In the beginning of the simulation, the tube temperature decreases and an initial condensate film is formed. Afterwards a drop is formed at the bottom of the tube, which eventually drips off. The simulation reaches a state, where condensate drips off periodically. The increase in thermal resistance due to the condensate film can be observed at the lower part of the tube, where the changes in the temperature field stand in accordance with the drip-off frequency.

![droplet_new](https://github.com/user-attachments/assets/d43b097b-2e4e-4778-8966-558e311556b7)


## Сonducted simulations
### 2D-Simulation of moving pure substance film condensation on a horizontal smooth tube

https://github.com/user-attachments/assets/f55ce8f3-b19b-441b-ab63-6ffcd1e31766

### 2D-Simulation of pure substance film condensation on a horizontal smooth tubes bundle
![Briggs_Uv04_U_OF](https://github.com/user-attachments/assets/c6071347-3797-4b13-b11a-0f26d2e75bd6)

## Contributing
We welcome contributions, particularly for:
 - Turbulence model improvements
 - Additional validation cases for LCA condensation model
 - Performance optimizations
 - solver stability

Please submit pull requests or open issues to discuss potential enhancements.


## Reference
* T. Kleiner, S. Rehfeldt, H. Klein, CFD model and simulation of pure substance condensation on horizontal tubes using the volume of fluid method, Int. J. Heat Mass Transf. 138 (2019) 420-431, [https://doi.org/10.1016/j.ijheatmasstransfer.2019.04.054](https://doi.org/10.1016/j.ijheatmasstransfer.2019.04.054).
* Lee Wen, A pressure iteration scheme for two-phase flow modeling, Multiphase Transport: Fundamentals, Reactor Safety, Applications, 1980, pp. 407–431
* K.B. Minko, V. I. Artemov, A.A. Klementiev, Simulation of Condensation of Stagnant or Moving Saturated Vapor on a Horizontal Tube Using the Volume-of-Fluid (VOF) Method, Thermal Engineering 2023, 70(3), 175-193, [https://link.springer.com/article/10.1134/S0040601523030059](https://link.springer.com/article/10.1134/S0040601523030059).
* Da Riva, Del Col, Numerical Simulation of Condensation in a Minichannel, Proceedings of the ASME 2009 Second International Conference on Micro/Nanoscale Heat and Mass Transfer. ASME 2009 Second International Conference on Micro/Nanoscale Heat and Mass Transfer, Volume 2. Shanghai, China. December 18–21, 2009. pp. 139-145. ASME, [https://asmedigitalcollection.asme.org/MNHT/proceedings-abstract/MNHMT2009/43901/139/334977](https://asmedigitalcollection.asme.org/MNHT/proceedings-abstract/MNHMT2009/43901/139/334977).
* K.B. Minko, V. I. Artemov, A.A. Klementiev, _, Thermal Engineering 2025, _, _, [_](_).
