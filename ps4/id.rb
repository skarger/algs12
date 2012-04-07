distinct_ids = 10**12
prob_all_different = 1
i = 1
while i <= distinct_ids do
    num = distinct_ids - i + 1
    den = distinct_ids * 1.0
    cur = num / den
    prob_all_different *= cur
    if prob_all_different < 1.0 / 2.0 then
        puts "#{i} #{prob_all_different}"
        break
    end
    i += 1
end

