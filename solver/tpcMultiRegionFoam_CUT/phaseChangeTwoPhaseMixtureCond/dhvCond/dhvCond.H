/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
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
    Foam::phaseChangeTwoPhaseMixtures::dhvCond

Description
    dhvCond condensation model.

SourceFiles
    dhvCond.C

\*--------------------------------------------------------------------*/

#ifndef dhvCond_H
#define dhvCond_H

#include "phaseChangeTwoPhaseMixtureCond.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace phaseChangeTwoPhaseMixtures
{

/*--------------------------------------------------------------------*\
                              Class dhvCond
\*--------------------------------------------------------------------*/

class dhvCond
:
    public phaseChangeTwoPhaseMixtureCond
{
    // Private data

        dimensionedScalar dhv_;
        dimensionedScalar Cc_;
        dimensionedScalar Cv_;
        dimensionedScalar he0_;

        dimensionedScalar mcCoeff_;
        dimensionedScalar mvCoeff_;

public:

    //- Runtime type information
    TypeName("dhvCond");


    // Constructors

        //- construct from components
        dhvCond
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~dhvCond()
    {}


    // Member Functions
        //- return the mass transfer rate for face fraction adjustment in Cell due to phase change
        virtual Pair<tmp<volScalarField> > mDotAlphal() const;

        //- return the mass transfer rate for pressure adjustment in Cell due to phase change
        virtual Pair<tmp<volScalarField> > mDotP() const;

        //- Correct the dhvCond phaseChange model
        virtual void correct();

        //- Read the transportProperties dictionary and update
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace phaseChangeTwoPhaseMixtures
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
