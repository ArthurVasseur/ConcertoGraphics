//
// Created by arthur on 30/06/2022.
//

#include "wrapper/PipelineLayout.hpp"
#include <stdexcept>

namespace Concerto::Graphics::Wrapper
{

	PipelineLayout::PipelineLayout(VkDevice device, std::size_t size,
			const std::vector<DescriptorSetLayout>& descriptorSetLayouts) : _device(device)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		VkPushConstantRange push_constant;
		std::vector<VkDescriptorSetLayout> set_layout;
		set_layout.reserve(descriptorSetLayouts.size());
		for (const auto & descriptorSetLayout : descriptorSetLayouts)
		{
			set_layout.push_back(descriptorSetLayout.get());
		}
		push_constant.offset = 0;
		push_constant.size = size;
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.flags = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = &push_constant;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0; //TODO reset
		pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
		pipelineLayoutCreateInfo.pSetLayouts = set_layout.data();

		if (vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}
	}

	PipelineLayout::~PipelineLayout()
	{
		vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
		_pipelineLayout = VK_NULL_HANDLE;
	}

	VkPipelineLayout PipelineLayout::get() const
	{
		return _pipelineLayout;
	}
}