///MAE_PhysBodyApplyCentralImpulse(body, relative, xvel, yvel, zvel)
/*
Accepts a vector or 3 seperate reals.
*/

if(argument_count == 3){
    var vec = argument[2];
    return external_call(global.MAB_BodySetApplyCentralImpulse, argument[0], vec[0], vec[1], vec[2], argument[1]);
}
return external_call(global.MAB_BodyApplyCentralImpulse, argument[0], argument[2], argument[3], argument[4], argument[1]);