-- curves for some fancy math

curves = {}

function curves.sin(from, to, at) 
    if at < 0 then 
        return from
    elseif at > 1 then 
        return to
    end

    return from + (to - from) * math.sin(at * math.pi / 2)
end
