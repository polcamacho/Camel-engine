#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	this->body = body;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();
	chassis.color = White;

	Cube seatback(info.seatback_size.x, info.seatback_size.y, info.seatback_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&seatback.transform);
	btVector3 b_offset(info.seatback_offset.x, info.seatback_offset.y, info.seatback_offset.z);
	b_offset = b_offset.rotate(q.getAxis(), q.getAngle());
	seatback.transform.M[12] += b_offset.getX();
	seatback.transform.M[13] += b_offset.getY();
	seatback.transform.M[14] += b_offset.getZ();
	seatback.color = Grey;

	Cube seat(info.seat_size.x, info.seat_size.y, info.seat_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&seat.transform);
	btVector3 c_offset(info.seat_offset.x, info.seat_offset.y, info.seat_offset.z);
	c_offset = c_offset.rotate(q.getAxis(), q.getAngle());
	seat.transform.M[12] += c_offset.getX();
	seat.transform.M[13] += c_offset.getY();
	seat.transform.M[14] += c_offset.getZ();
	seat.color = Grey;

	Cube lateral_right(info.lateral_right_size.x, info.lateral_right_size.y, info.lateral_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&lateral_right.transform);
	btVector3 d_offset(info.lateral_right_offset.x, info.lateral_right_offset.y, info.lateral_right_offset.z);
	d_offset = d_offset.rotate(q.getAxis(), q.getAngle());
	lateral_right.transform.M[12] += d_offset.getX();
	lateral_right.transform.M[13] += d_offset.getY();
	lateral_right.transform.M[14] += d_offset.getZ();
	lateral_right.color = White;

	Cube lateral_left(info.lateral_left_size.x, info.lateral_left_size.y, info.lateral_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&lateral_left.transform);
	btVector3 e_offset(info.lateral_left_offset.x, info.lateral_left_offset.y, info.lateral_left_offset.z);
	e_offset = e_offset.rotate(q.getAxis(), q.getAngle());
	lateral_left.transform.M[12] += e_offset.getX();
	lateral_left.transform.M[13] += e_offset.getY();
	lateral_left.transform.M[14] += e_offset.getZ();
	lateral_left.color = White;

	Cube front(info.front_size.x, info.front_size.y, info.front_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&front.transform);
	btVector3 f_offset(info.front_offset.x, info.front_offset.y, info.front_offset.z);
	f_offset = f_offset.rotate(q.getAxis(), q.getAngle());
	front.transform.M[12] += f_offset.getX();
	front.transform.M[13] += f_offset.getY();
	front.transform.M[14] += f_offset.getZ();
	front.color = White;

	Cube back(info.back_size.x, info.back_size.y, info.back_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&back.transform);
	btVector3 g_offset(info.back_offset.x, info.back_offset.y, info.back_offset.z);
	g_offset = g_offset.rotate(q.getAxis(), q.getAngle());
	back.transform.M[12] += g_offset.getX();
	back.transform.M[13] += g_offset.getY();
	back.transform.M[14] += g_offset.getZ();
	back.color = White;

	Cube aileron_right(info.aileron_right_size.x, info.aileron_right_size.y, info.aileron_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron_right.transform);
	btVector3 h_offset(info.aileron_right_offset.x, info.aileron_right_offset.y, info.aileron_right_offset.z);
	h_offset = h_offset.rotate(q.getAxis(), q.getAngle());
	aileron_right.transform.M[12] += h_offset.getX();
	aileron_right.transform.M[13] += h_offset.getY();
	aileron_right.transform.M[14] += h_offset.getZ();
	aileron_right.color = Red;
	
	Cube aileron_left(info.aileron_left_size.x, info.aileron_left_size.y, info.aileron_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron_left.transform);
	btVector3 i_offset(info.aileron_left_offset.x, info.aileron_left_offset.y, info.aileron_left_offset.z);
	i_offset = i_offset.rotate(q.getAxis(), q.getAngle());
	aileron_left.transform.M[12] += i_offset.getX();
	aileron_left.transform.M[13] += i_offset.getY();
	aileron_left.transform.M[14] += i_offset.getZ();
	aileron_left.color = Red;

	Cube aileron(info.aileron_size.x, info.aileron_size.y, info.aileron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron.transform);
	btVector3 j_offset(info.aileron_offset.x, info.aileron_offset.y, info.aileron_offset.z);
	j_offset = j_offset.rotate(q.getAxis(), q.getAngle());
	aileron.transform.M[12] += j_offset.getX();
	aileron.transform.M[13] += j_offset.getY();
	aileron.transform.M[14] += j_offset.getZ();
	aileron.color = Green;

	Cube handelbar(info.handlebar_size.x, info.handlebar_size.y, info.handlebar_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&handelbar.transform);
	btVector3 k_offset(info.handlebar_offset.x, info.handlebar_offset.y, info.handlebar_offset.z);
	k_offset = k_offset.rotate(q.getAxis(), q.getAngle());
	handelbar.transform.M[12] += k_offset.getX();
	handelbar.transform.M[13] += k_offset.getY();
	handelbar.transform.M[14] += k_offset.getZ();
	handelbar.color = Violet;

	Cube handles_right(info.handles_right_size.x, info.handles_right_size.y, info.handles_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&handles_right.transform);
	btVector3 l_offset(info.handles_right_offset.x, info.handles_right_offset.y, info.handles_right_offset.z);
	l_offset = l_offset.rotate(q.getAxis(), q.getAngle());
	handles_right.transform.M[12] += l_offset.getX();
	handles_right.transform.M[13] += l_offset.getY();
	handles_right.transform.M[14] += l_offset.getZ();
	handles_right.color = Gold;

	Cube handles_left(info.handles_left_size.x, info.handles_left_size.y, info.handles_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&handles_left.transform);
	btVector3 m_offset(info.handles_left_offset.x, info.handles_left_offset.y, info.handles_left_offset.z);
	m_offset = m_offset.rotate(q.getAxis(), q.getAngle());
	handles_left.transform.M[12] += m_offset.getX();
	handles_left.transform.M[13] += m_offset.getY();
	handles_left.transform.M[14] += m_offset.getZ();
	handles_left.color = Gold;

	chassis.Render();
	seat.Render();
	seatback.Render();
	lateral_right.Render();
	lateral_left.Render();
	front.Render();
	back.Render();
	aileron_right.Render();
	aileron_left.Render();
	aileron.Render();
	handelbar.Render();
	handles_right.Render();
	handles_left.Render();

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

vec3 PhysVehicle3D::ForwardVector() const
{
	vec3 vec;
	btVector3 FwVector = vehicle->getForwardVector();
	vec.Set(FwVector.getX(), FwVector.getY(), FwVector.getZ());
	return vec;
}