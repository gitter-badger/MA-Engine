///MAE_PhysBodySetLinearVelocity(body, velocity)
///MAE_PhysBodySetLinearVelocity(body, x, y, z)

/**
 * Description:
 *   Sets the linear velocity of the physics body.
 *
 * Arguments:
 *   [0] - Body
 *   [1] - Velocity vector
 *
 * Returns:
 *   Success
 */

if(argument_count == 2){
    var vec = argument[1];
    return external_call(global.MAB_BodySetLinearVelocity, argument[0], vec[0], vec[1], vec[2]);
}
return external_call(global.MAB_BodySetLinearVelocity, argument[0], argument[1], argument[2], argument[3]);
