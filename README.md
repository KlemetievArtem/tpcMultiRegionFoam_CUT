# (IN PROCESS)

# tpcMultiRegionFoam_CUT &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp;&nbsp;
tpcMultiRegionFoamCUT solver is modification of [tpcMultiRegionFoam](https://github.com/ThomasKleiner/tpcMultiRegionFoam) solver with:
 - Different condensation models
 - Turbulence modeling
 - Post-processing functions
 - Various improvements and fixes

tpcMultiRegionFoam_CUT is CFD Solver for heat transfer simulations between solid and fluid regions with implemented models for thermal phase change of pure substances in fluid regions (Lee model - LC; Kleiner, Rehfeldt, Klein model - KRK; Lee model with AUTOMATIC constant calculation (Minko model) - LCA), turbulence model in vapor phase and calculation of wallHeatFluxes on patches. 

 ![itf](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/TPD.jpg)


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
 - cross-verification of LCA condenstion model using OpenFOAM, Ansys Fluent and [ANES](http://anes.ch12655.tmweb.ru/index.php/primery) perfermed in [Minko, Artemov, Klementiev 2025](). 

# Citing This Work
If using the LCA condensation model or concepts from this repository leads to publication, please cite: [Minko, Artemov, Klementiev 2022](https://link.springer.com/article/10.1134/S0040601523030059) 
Further refinement of the solver is welcome (especially turbulence model)!

## Getting Started

For installation, the [OpenFOAM 6 libraries](https://github.com/OpenFOAM/OpenFOAM-6) have to be loaded. 

All necessary applications for the solver installation can be installed and uninstalled by the supplied [AllUnowmake_CUT](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/solver/AllUnowmake_CUT) and [AllUnowclean_CUT](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/solver/AllUnowclean_CUT) shell scripts, respectively. The scripts are located in the [solver](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/solver) folder.

Copy the content of the solver directory in your desired user applications folder and run the [AllUnowmake_CUT](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/solver/AllUnowclean_CUT) script.
```
Working folder:/home/<user>/OpenFOAM/<user>-6/applications
        
$ cd /home/<user>/OpenFOAM/<user>-6/applications
$ cd solver
$ chmod 700 AllUnow*
$ ./AllUnowmake_CUT
```

## Provided Test Cases
Two test cases, are provided

### Stefan proplem

![Stepfan](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/Stefane.png)

### 2D-Simulation of pure substance film condensation on a horizontal smooth tube
![droplet_new](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/droplet_new.gif)


## Сonducted simulations
### Comparison of 2D and 3D cases

The dynamics of droplet detachment obtained from the results of 2D and 3D numerical modeling differ. 
However, the averaged value of the heat transfer coefficient on cylinder is practically identical in the compared cases.
![3D_2D](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/3D_2D.png)

### 2D-Simulation of moving pure substance film condensation on a horizontal smooth tube

The increase of vapor velocity intensify heat transfer in accordance with experiment.
![U_2_4_6](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/U_2_4_6_pink2.gif)


### 2D-Simulation of pure substance film condensation on a horizontal smooth tubes bundle

Condensation on a tube bundle segment can be modeled numerically
![Briggs_Uv04_U_OF](https://github.com/KlemetievArtem/tpcMultiRegionFoam_CUT/blob/main/fig_vid/Briggs_Uv04_U_OF.gif)

## Contributing
We welcome contributions, particularly for:
 - Turbulence model improvements
 - Additional validation cases for LCA condensation model
 - Performance optimizations
 - Solver stability

Please submit pull requests or open issues to discuss potential enhancements.


## Reference
* T. Kleiner, S. Rehfeldt, H. Klein, CFD model and simulation of pure substance condensation on horizontal tubes using the volume of fluid method, Int. J. Heat Mass Transf. 138 (2019) 420-431, [https://doi.org/10.1016/j.ijheatmasstransfer.2019.04.054](https://doi.org/10.1016/j.ijheatmasstransfer.2019.04.054).
* Lee Wen, A pressure iteration scheme for two-phase flow modeling, Multiphase Transport: Fundamentals, Reactor Safety, Applications, 1980, pp. 407–431
* K.B. Minko, V. I. Artemov, A.A. Klementiev, Simulation of Condensation of Stagnant or Moving Saturated Vapor on a Horizontal Tube Using the Volume-of-Fluid (VOF) Method, Thermal Engineering 2023, 70(3), 175-193, [https://link.springer.com/article/10.1134/S0040601523030059](https://link.springer.com/article/10.1134/S0040601523030059).
* Da Riva, Del Col, Numerical Simulation of Condensation in a Minichannel, Proceedings of the ASME 2009 Second International Conference on Micro/Nanoscale Heat and Mass Transfer. ASME 2009 Second International Conference on Micro/Nanoscale Heat and Mass Transfer, Volume 2. Shanghai, China. December 18–21, 2009. pp. 139-145. ASME, [https://asmedigitalcollection.asme.org/MNHT/proceedings-abstract/MNHMT2009/43901/139/334977](https://asmedigitalcollection.asme.org/MNHT/proceedings-abstract/MNHMT2009/43901/139/334977).
* K.B. Minko, V. I. Artemov, A.A. Klementiev, _, Thermal Engineering 2025, _, _, [_](_).
