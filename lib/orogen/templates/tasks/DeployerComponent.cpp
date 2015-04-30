#include <rtt/Component.hpp>

#include <orogen/deployer.hpp>

#include <class_loader/class_loader.h>

<% deployable_tasks = project.self_tasks.find_all { |t| !t.abstract? } %>
<% deployable_tasks.each do |task| %>
#include <<%= task.header_file %>>
<% end %>

ORO_CREATE_COMPONENT_TYPE();
<% deployable_tasks.each do |task| %>
ORO_LIST_COMPONENT_TYPE( <%= task.name %> );
<% end %>

namespace orogen
{

<% deployable_tasks.each do |task| %>
    RTT::TaskContext* create_<%= task.name.gsub(/[^\w]/, '_') %>(std::string const& instance_name)
    {
        return new <%= task.name %>(instance_name);
    }
<% end %>
}

<% deployable_tasks.each do |task| %>
<% task.full_namespace.split("::").each do |space| %>
namespace <%= space %>{
<% end %>

    class <%= task.basename %>Deployer : public orogen::Deployer{
        virtual ::RTT::TaskContext* getTask(const std::string &instance_name) const{
            return new <%= task.name %>(instance_name);
        };
    };
<% task.full_namespace.split("::").each do |space| %>
}
<% end %>

CLASS_LOADER_REGISTER_CLASS(<%= task.full_namespace %>::<%= task.basename %>Deployer, ::orogen::Deployer);

<% end %>
