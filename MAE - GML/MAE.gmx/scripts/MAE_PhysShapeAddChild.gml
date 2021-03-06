///MAE_PhysShapeAddChild(parent_shape, child_shape, position, rotation)
///MAE_PhysShapeAddChild(parent_shape, child_shape, x, y, z, rotation)

/**
 * Description:
 *   Adds a child shape to a compound shape with an offset and rotation.
 *
 * Arguments:
 *   [0] - Parent shape
 *   [1] - Child shape
 *   [2] - Position as vector
 *   [3] - Rotation as quaternion
 *
 * Returns:
 *   Child index
 */

var rot = argument[3];
if(argument_count == 4){
    var vec = argument[2];
    return external_call(global.MAB_ShapeAddChild, argument[0], argument[1], vec[0], vec[1], vec[2], rot[0], rot[1], rot[2], rot[3]);
}
return external_call(global.MAB_ShapeAddChild, argument[0], argument[1], argument[2], argument[3], argument[4], rot[0], rot[1], rot[2], rot[3]);

