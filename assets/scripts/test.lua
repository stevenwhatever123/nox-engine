function tick(dt, current_time)
	obj_pos = self:GetTransformComponent();

	obj_pos.x = 100*math.cos(current_time);
	obj_pos.z = 20*math.sin(current_time);
end
