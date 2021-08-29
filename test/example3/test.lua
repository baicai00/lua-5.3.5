local function f(...)
    --local t = {...} --变量t是一张表,里面存放了所有的入参
    --local q = ...   --变量q将只会保存第一个入参
    --使用ipairs函数循环打印可变参数列表时,需要使用ipairs{...}
    for i,v in ipairs{...} do
        print("yield ret:", coroutine.yield(co, v*10))
    end
end

co = coroutine.create(f)
print("resume ret:", coroutine.resume(co, 1,2,3,4))
print("resume ret:", coroutine.resume(co, 1))
print("resume ret:", coroutine.resume(co, 2))
print("resume ret:", coroutine.resume(co, 3))
print("resume ret:", coroutine.resume(co, 4))
print(coroutine.status(co))

--[[
resume ret: 10
yield ret:  1
resume ret: 20
]]
