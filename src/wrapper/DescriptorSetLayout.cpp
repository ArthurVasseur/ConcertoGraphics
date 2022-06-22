//
// Created by arthur on 16/06/22.
//


#include "wrapper/DescriptorSetLayout.hpp"
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{

	DescriptorSetLayout::DescriptorSetLayout(VkDevice device, std::vector<VkDescriptorSetLayoutBinding> bindings)
			: _device(device), _layout(VK_NULL_HANDLE)
	{
		VkDescriptorSetLayoutCreateInfo createInfo = {};
		createInfo.bindingCount = bindings.size();
		createInfo.flags = 0;
		createInfo.pNext = nullptr;
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.pBindings = bindings.data();
		if (vkCreateDescriptorSetLayout(device, &createInfo, nullptr, &_layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(_device, _layout, nullptr);
	}

	VkDescriptorSetLayout DescriptorSetLayout::get() const
	{
		return _layout;
	}

}