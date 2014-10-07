#ifndef OROGEN_<%= typekit.name %>_TYPE_FWD_HPP
#define OROGEN_<%= typekit.name %>_TYPE_FWD_HPP

<% all_types = type_sets.types.dup
   all_types.dup.each do |type|
       all_types << typekit.intermediate_type_for(type)
   end
   all_types.map! do |type|
       while type.respond_to?(:deference)
           type = type.deference
       end
       type
   end
   all_types = all_types.
       find_all { |t| !(t <= Typelib::NumericType) && !(t <= Typelib::ContainerType) }.
       to_set
   need_full_definition, forward_declared = all_types.partition do |type|
       result = !(type < Typelib::CompoundType) || type.name =~ /</ || (type.opaque? && type.metadata.get('opaque_is_typedef').include?('1'))
       result
   end
%>

<%= includes = need_full_definition.inject(Set.new) do |all, type|
       all |= typekit.include_for_type(type).to_set
    end
    typekit.cxx_gen_includes(*includes)
%>

// This is needed so that we don't need to explicitly include the headers for
// the vector element types
#include <vector>

<% current_namespace = '/' %>
<% forward_declared.sort_by(&:name).each do |type| %>
<%= Orocos::Generation.adapt_namespace(current_namespace, type.namespace) %>
<%     current_namespace = type.namespace %>
class <%= type.basename %>;
<% end %>
<%= Orocos::Generation.adapt_namespace(current_namespace, '/') %>

#endif

