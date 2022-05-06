-- function dump_rev(obj)


-- 	if getmetatable(obj) == nil then
-- 		for name,val in pairs(obj) do
-- 			print(name, val)
-- 			if(type(val) == 'table') then
-- 				dump_rev(val)
-- 			end
-- 		end
-- 	else
-- 		for name,val in pairs(getmetatable(obj)) do
-- 			print(name, val)
-- 			if(type(val) == 'table') then
-- 				dump_rev(val)
-- 			end
-- 		end
-- 	end

-- end


-- dump_rev(PositionComponent)

function tick(dt)
	obj_pos = self:GetPositionComponent();
	print(obj_pos.x, obj_pos.y, obj_pos.z)
end
