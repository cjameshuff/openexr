
require 'openexr_native'

module Imath

# Handle binary operations with a given type as right hand side:
# coerce LHS to CoercedLHS and implement operators to call appropriate functions of RHS.
COERCED_OPERATORS = {
    :+ => "add", :- => "sub", :* => "mul", :/ => "div", :% => 'mod', :** => 'exp',
    :| => 'bar', :& => 'amp', :|| => 'dbar', :&& => 'damp', :^ => 'hat',
    :<< => 'lshift', :>> => 'rshift'
}
class ScalarLHS
    def initialize(original)
        @original = original
    end
    OPERATORS.each {|op, name|
        define_method(op) {|val| val.send("scalar_#{name}".to_sym, @original)}
    }
end
class Vec3LHS
    def initialize(original)
        @original = original
    end
    OPERATORS.each {|op, name|
        define_method(op) {|val| val.send("vec3_#{name}".to_sym, @original)}
    }
end
class Mat33LHS
    def initialize(original)
        @original = original
    end
    OPERATORS.each {|op, name|
        define_method(op) {|val| val.send("mat33_#{name}".to_sym, @original)}
    }
end

class Quatd
    def coerce(lhs)
        case(lhs)
        when Vec3d, Vec3i, Array
            [Vec3LHS.new(lhs), self]
        when Matrix33
            [Mat33LHS.new(lhs), self]
        else
            raise TypeError, "#{self.class} can't be coerced into #{lhs.class}"
        end
    end
end

class Matrix33d
    def coerce(lhs)
        case(lhs)
        when Vec3d, Vec3i, Array
            [Vec3LHS.new(lhs), self]
        else
            raise TypeError, "#{self.class} can't be coerced into #{lhs.class}"
        end
    end
end

class Matrix44d
    def coerce(lhs)
        case(lhs)
        when Vec3d, Vec3i, Array
            [Vec3LHS.new(lhs), self]
        else
            raise TypeError, "#{self.class} can't be coerced into #{lhs.class}"
        end
    end
end

end # module Imath