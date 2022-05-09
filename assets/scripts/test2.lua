function tick(dt, current_time)
	obj_pos = self:GetPositionComponent();
	obj_pos.z = 10*math.sin(current_time);
	obj_pos.x = 10*math.cos(current_time);
end
