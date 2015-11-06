///MAE_PhysShapeSetScaling(shape, x, y, z)
/*
Accepts a vector or 3 seperate reals.
*/

if(argument_count == 2){
    var vec = argument[1];
    return external_call(global.MAB_ShapeSetScaling, argument[0], vec[0], vec[1], vec[2]);
}
return external_call(global.MAB_ShapeSetScaling, argument[0], argument[1], argument[2], argument[3]);
