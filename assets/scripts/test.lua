function tick(dt, current_time)
	obj_pos = self:GetPositionComponent();

	obj_pos.x = 100*math.cos(current_time);
	obj_pos.z = 100*math.sin(current_time);
end
