///MAE_VectorIsVec4(n);

/**
 * Description:
 *   Checks if the ID given is a Vector4.
 * 
 * Arguments:
 *   1 - Vector ID.
 *
 * Return:
 *   True / False value.
 */
 
return is_array(argument0) && array_length_1d(argument0) == 4;
