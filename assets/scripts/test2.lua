function tick(dt, current_time)
	obj_pos = self:GetPositionComponent();
	obj_pos.x = 20*math.sin(current_time);
	obj_pos.ry = 20*math.cos(current_time);
end
