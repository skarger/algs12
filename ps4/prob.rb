
class Integer
  # binomial coefficient: n C k
  def choose(k)
    # n!/(n-k)!
    pTop = (self-k+1 .. self).inject(1, &:*) 
    # k!
    pBottom = (2 .. k).inject(1, &:*)
    pTop / pBottom
  end
end

# 1
# experimentally show that probability of
# super-sketch matches is r
r = 0.9
max = 14
p = 0
i = 0
while i <= max do
    eq1 = max.choose(i)
    eq2 = r**i
    eq3 = (1-r)**(max-i)
    eq4 = i / Float(max)
    p += eq1 * eq2 * eq3 * eq4
    puts "#{i}\t#{p}"
    i = i+1
end
puts ""



# 2
# calculate probability of 2 of more
# super-sketch matches out of 6
# across a range or r values
r = 0.01
max = 6 
while r <= 1.0 do
    p = 0
    i = 2
    while i <= max do
    	eq1 = max.choose(i)
	eq2 = r**i
        eq3 = (1-r)**(max-i)
        p += eq1 * eq2 * eq3
        i = i+1
    end
    puts "#{r}\t#{p}"
    r += 0.01
end

