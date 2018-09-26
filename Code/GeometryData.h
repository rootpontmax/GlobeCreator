////////////////////////////////////////////////////////////////////////////////////////////////////
// Types of geometry representation
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


#include "Types.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
bool            operator<( const SEdge& lhs, const SEdge& rhs );
SVert           operator+( const SVert& lhs, const SVert& rhs );
void            ReportIcosahedron( const SIcosahedron& ico );
void            CheckIcosahedron( const SIcosahedron& ico );
void            NormalizeIcosahedron( SIcosahedron *pIco );
void            CalcCoordinates( SIcosahedron *pIco );
void            SaveIcosahedronGeom( const SIcosahedron& ico, const char *pFilename );
void            SaveIcosahedronData( const SIcosahedron& ico, const char *pFilename );
SIcosahedron    CreateIcosahedron();
SIcosahedron    SplitIcosahedron( SIcosahedron& oldIco );
////////////////////////////////////////////////////////////////////////////////////////////////////
