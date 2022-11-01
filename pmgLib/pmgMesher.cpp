#include "pmgMesher.h"

bool pmg::skip(int i, int last, node_skip nskip) {
	if (nskip == node_skip::none) {
		return false;
	}
	if (i == 0) {
		return nskip == node_skip::first || nskip == node_skip::first_and_last || ((int)nskip >= 2);
	}
	else if (i == (last - 1) && ((int)nskip < 2)) {
		return nskip == node_skip::last || nskip == node_skip::first_and_last;
	}
	else if ((int)nskip >= 2) {
		return !(bool)(i % (int)nskip);
	}
	else {
		return false;
	}
}