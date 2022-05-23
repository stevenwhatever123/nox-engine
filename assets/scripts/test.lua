

function tick(dt, current_time)
	obj_pos = self:GetCameraComponent();
	cam = obj_pos:camera();
	cam_pos = cam:pos();
	cam.yaw = math.sin(current_time);
	cam.pitch = math.sin(current_time);
end
