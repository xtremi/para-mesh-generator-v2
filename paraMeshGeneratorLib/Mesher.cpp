#include "Mesher.h"
int Mesher::nodeID1 = 1;
FEAwriter* Mesher::writer = nullptr;


void RowMesher2D::writeElements(NodeIterator1D* nodeIt1, NodeIterator1D* nodeIt2)
{
	int n[4];
	n[0] = nodeIt1->first();
	n[1] = nodeIt2->first();
	n[2] = nodeIt2->next();
	n[3] = nodeIt1->next();


	while (n[1] > 0 && n[2] > 0) {		
		//	0-------1 
		//	|       |
		//	3-------2
		Mesher::writer->write4nodedShell(n);		
		n[0] = n[3];
		n[1] = n[2];
		n[3] = nodeIt1->next();
		n[2] = nodeIt2->next();
	}
}


void RowMesher3D::writeElements(NodeIterator2D* nodeIt1, NodeIterator2D* nodeIt2) {
	int n[8];

	nodeIt1->first4(n[0], n[1], n[2], n[3]);
	nodeIt2->first4(n[4], n[5], n[6], n[7]);

	for (bool more = (nodeIt1->first4(n[0], n[1], n[2], n[3]) && nodeIt2->first4(n[4], n[5], n[6], n[7]));
			more;
			more = (nodeIt1->next4(n[0], n[1], n[2], n[3]) && nodeIt2->next4(n[4], n[5], n[6], n[7]))) 
	{
		Mesher::writer->write8nodedHexa(n);
	}

}
