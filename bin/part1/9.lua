 function treaverse_global_env(curtable,level)
    for key,value in pairs(curtable or {}) do
  local prefix = string.rep(" ",level*5)
  print(string.format("%s%s(%s)",prefix,key,type(value)))

  --注意死循环
  if (type(value) == "table" ) and key ~= "_G" and (not value.package) then
      treaverse_global_env(value,level + 1)
  elseif (type(value) == "table" ) and (value.package) then
      print(string.format("%sSKIPTABLE:%s",prefix,key))
  end 
    end 
 end

 a = "asjfaowigeoawhgeoiwajgeoiwahgeaoigjeoiahgwieag"

treaverse_global_env(_G,0)

