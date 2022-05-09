function tick(dt, current_time)
	obj_pos = self:GetPositionComponent();

	obj_pos.x = 10*math.cos(current_time);
end
