/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::incompressibleTwoPhaseMixtureCond

Description
    A two-phase incompressible transportModel

SourceFiles
    incompressibleTwoPhaseMixtureCond.C

\*---------------------------------------------------------------------------*/

#ifndef incompressibleTwoPhaseMixtureCond_H
#define incompressibleTwoPhaseMixtureCond_H

#include "incompressible/transportModel/transportModel.H"
#include "incompressible/viscosityModels/viscosityModel/viscosityModel.H"
#include "twoPhaseMixture.H"
#include "IOdictionary.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class incompressibleTwoPhaseMixtureCond Declaration
\*---------------------------------------------------------------------------*/

class incompressibleTwoPhaseMixtureCond
:
    public IOdictionary,
    public transportModel,
    public twoPhaseMixture
{
protected:

    // Protected data

        autoPtr<viscosityModel> nuModel1_;
        autoPtr<viscosityModel> nuModel2_;

        dimensionedScalar rho1_;
        dimensionedScalar rho2_;

        dimensionedScalar DT1_;
        dimensionedScalar DT2_;

        dimensionedScalar Cp1_;
        dimensionedScalar Cp2_;

        const volVectorField& U_;
        const surfaceScalarField& phi_;

        volScalarField nu_;


    // Private Member Functions

        //- Calculate and return the laminar viscosity
        void calcNu();


public:

    TypeName("incompressibleTwoPhaseMixtureCond");


    // Constructors

        //- Construct from components
        incompressibleTwoPhaseMixtureCond
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );
        
        incompressibleTwoPhaseMixtureCond
        (
            const volVectorField& U,
            const surfaceScalarField& phi,
            const dictionary& dict
        );


    //- Destructor
    virtual ~incompressibleTwoPhaseMixtureCond()
    {}


    // Member Functions
    
        static const word dictName;

        //- Return const-access to phase1 viscosityModel
        const viscosityModel& nuModel1() const
        {
            return nuModel1_();
        }

        //- Return const-access to phase2 viscosityModel
        const viscosityModel& nuModel2() const
        {
            return nuModel2_();
        }

        //- Return const-access to phase1 density
        const dimensionedScalar& rho1() const
        {
            return rho1_;
        }

        //- Return const-access to phase2 density
        const dimensionedScalar& rho2() const
        {
            return rho2_;
        };

        //- Return const-access to phase1 thermoconductivity
        const dimensionedScalar& DT1() const
        {
            return DT1_;
        }

        //- Return const-access to phase2 thermoconductivity
        const dimensionedScalar& DT2() const
        {
            return DT2_;
        };

        //- Return const-access to phase1 thermoconductivity
        const dimensionedScalar& Cp1() const
        {
            return Cp1_;
        }

        //- Return const-access to phase2 thermoconductivity
        const dimensionedScalar& Cp2() const
        {
            return Cp2_;
        };

        //- Return const-access to the mixture velocity
        const volVectorField& U() const
        {
            return U_;
        }

        //- Return the dynamic laminar viscosity
        tmp<volScalarField> mu() const;

        //- Return the face-interpolated dynamic laminar viscosity
        tmp<surfaceScalarField> muf() const;

        //- Return the kinematic laminar viscosity
        virtual tmp<volScalarField> nu() const
        {
            return nu_;
        }

        //- Return the laminar viscosity for patch
        virtual tmp<scalarField> nu(const label patchi) const
        {
            return nu_.boundaryField()[patchi];
        }

        //- Return the face-interpolated kinematic laminar viscosity
        tmp<surfaceScalarField> nuf() const;

        //- Correct the laminar viscosity
        virtual void correct()
        {
            calcNu();
        }

        //- Read base transportProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
