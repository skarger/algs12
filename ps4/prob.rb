
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

r = 0.37
max = 14

#while r <= 1.0 do
    p = 0
    i = 0
    while i <= max do
        eq1 = r**i
        eq2 = (1-r)**(max-i)
        eq3 = max.choose(i)
        #p += eq1 * eq2 * eq3
        eq4 = i / 14.0
        p += eq1 * eq2 * eq3 * eq4
        
        puts "#{r}\t#{i}\t#{p}"
        i = i+1
    end
#    r += 0.01
#end
