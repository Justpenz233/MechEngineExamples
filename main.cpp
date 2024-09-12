// Project DIR,use as the root of the project
#ifndef PROJECT_DIR
#define PROJECT_DIR ""
#endif

#include "Editor.h"

#include "BasicShapesExample.h"		 // This example demonstrates how to create baisc shapes as mesh
#include "DebugDrawExample.h"		 // This example demonstrates how to use the debug draw functions
#include "CurveExample.h"			 // This example demonstrates how to create a curve as mesh
#include "MeshBooleanTest.h"		 // This example demonstrates how to use the mesh boolean(CSG) functions
#include "MeshNormalExample.h"		 // This example demonstrates how to calculate the normal of a mesh, and compare the normal for rendering
#include "ParametricMeshExamples.h"	 // This example demonstrates how to create parametric mesh by given a arbitrary mesh or use bulit-in parametric mesh
#include "SphericalLinkageExample.h" // This example demonstrates how to create a spherical linkage
#include "TestRendering.h"
#include "WireFrameMaterialExample.h" // This example demonstrates how to create a wireframe material and use alpha blending material
#include "SplineExample.h"			  // This example demonstrates how to create a spline mesh
#include "IntersectionExample.h"	  // This example demonstrates how to use the intersection functions
#include "ConvexHullExample.h"		  // This example demonstrates how to use the convex hull function for 3D mesh
#include "MetaDataExample.h"	      // This example demonstrates how to use the metadata system, e.g. reflection system in Editor
#include "ProjectToSurfaceExample.h"  // This example demonstrates how to project a 3D point to 2D paramtric surface
#include "BuiltinSurfaces.h"		  // This example shows all the built-in parametric surfaces in the engine(can be sampled by (U, V,thickness))
#include "ExtrusionCurve.h" 		 //  This example demonstrates how to extrude a curve by a 2d simple Contour. For example, a torus can be created by extruding a circle along a circle.
#include "SolidifyMeshExample.h".    // This example demonstrates how to solidify a mesh by given thickness
#include "OrientedSurfaceExample.h"
#include "CustomShaderExample.h"
int main(int argc, char *argv[])
{
    GEditor.Init(argv[0], PROJECT_DIR);
    GEditor.LoadWorld(ProjectToSurfaceExample());
    GEditor.Start();
}