local count = 0

callbacks = {
	StartElement = function (parser, tagname)
		print("+ ", string.rep(" ", count), tagname, "\n")
		count = count + 1
	end,

	EndElement = function (parser, tagname)
		count = count - 1
		print("- ", string.rep(" ", count), tagname, "\n")
	end,
}

p = lxp.new(callbacks)

for l in io.lines() do
	assert(p:parse(l))
	assert(p:parse("\n"))
end

assert(p:parse())
p:close()
