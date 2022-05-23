function tick(dt, current_time)
	obj_pos = self:GetPositionComponent();
	obj_pos.x = 20*math.sin(current_time);
	obj_pos.rx = 2*math.cos(current_time);
end
