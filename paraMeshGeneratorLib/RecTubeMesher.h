#pragma once
#include "Mesher.h"
#include "FeaWrite.h"

class RecTubeMesher: public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&		spos,
		int					nNodesInner,
		int					nLayers,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		int					nNodesPerimeter,
		int					nLayers,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		int					nNodesWidth,
		int					nNodesHeight,
		int					nLayers,
		const RecTubeSize&	size,
		plane				pln);

	static void writeElements();
};

class RecTubeMesherRef : public Mesher
{
public:
	static void writeNodes(
		const MeshCsys&		spos,
		int					nNodesInner,
		int					nRefs,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		int					nNodesPerimeter,
		int					nRefs,
		const RecTubeSize&	size,
		plane				pln);

	static void writeNodes2(
		const MeshCsys&		spos,
		int					nNodesWidth,
		int					nNodesHeight,
		int					nLayers,
		const RecTubeSize&	size,
		plane				pln);

	static void writeElements();
};