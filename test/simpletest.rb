#!/usr/bin/env ruby

#$: << '.'

require 'openexr'
include Imath

# puts test1
# puts RRay::test1
# puts RRay::test1
# puts RRay::test1
# puts RRay::test1

# TODO:

# initialize()
# initialize(dir, len)
# initialize([x, y...])
# initialize([x, y...], len)
# initialize(x, y)
# initialize(x, y, z) (V3, V4 only)
# initialize(x, y, z, w) (V4 only)
# vec[n]
# vec[n] =
# vec.normalize()
# vec.normalized()
# vec.length()
# vec.length2()
# vec.dot()
# vec.cross() (V3 only)
# + - * /

testvec = V3d.new(1.2, 3.4, 5.6)
testvec2 = V3d.new(8.8, 6.6, 4.4)

puts "V3d.new: #{V3d.new.to_s}"
puts "V3d.new(1, 2, 3): #{V3d.new(1, 2, 3).to_s}"
puts "V3d.new([4, 5, 6]): #{V3d.new([4, 5, 6]).to_s}"
puts "V3d.new(testvec2): #{V3d.new(testvec2).to_s}"

puts "testvec: #{testvec.to_s}"
puts "testvec2: #{testvec2.to_s}"
puts "testvec + testvec2: #{(testvec + testvec2).to_s}"
