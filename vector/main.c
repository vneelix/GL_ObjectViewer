#include "vector.h"

int	main() {
	t_matrix4x4 m4x4 = (t_matrix4x4){
		3, 1, 2, 0,
		2, 1, 3, 0,
		3, 1, 6, 0,
		0, 0, 0, 1
	};
	t_float4 v = mul(invert(m4x4), (t_float4){1.3, 1.4, 1.5, 0});
	return (0);
}
