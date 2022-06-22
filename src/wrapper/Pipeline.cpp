//
// Created by arthur on 10/06/22.
//


#include <iostream>
#include "wrapper/Pipeline.hpp"
#include "wrapper/VulkanInitializer.hpp"

namespace Concerto::Graphics::Wrapper
{

	Pipeline::Pipeline(VkDevice& device, const PipeLineInfo& pipeLineInfo) : _device(device),
																			 _pipeLineInfo(pipeLineInfo),
																			 _createInfo()
	{
		_createInfo.viewportState = buildViewportState();
		_createInfo.colorBlend = buildColorBlendState();
		_createInfo.pipelineCreateInfo = {};
		_createInfo.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	}

	VkPipeline Pipeline::buildPipeline(const VkRenderPass& renderPass) const
	{
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		VkPipelineViewportStateCreateInfo viewportStateCreateInfo = buildViewportState();
		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = buildColorBlendState();
		VkPipeline newPipeline{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.stageCount = _pipelineInfo._shaderStages.size();
		pipelineInfo.pStages = _pipelineInfo._shaderStages.data();
		pipelineInfo.pVertexInputState = &_pipelineInfo._vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &_pipelineInfo._inputAssembly;
		pipelineInfo.pViewportState = &viewportStateCreateInfo;
		pipelineInfo.pRasterizationState = &_pipelineInfo._rasterizer;
		pipelineInfo.pMultisampleState = &_pipelineInfo._multisampling;
		pipelineInfo.pColorBlendState = &colorBlendStateCreateInfo;
		pipelineInfo.layout = _pipelineInfo._pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &_pipelineInfo._depthStencil;
		if (vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
		{
			std::cerr << "failed to create pipeline\n";
			return VK_NULL_HANDLE;
		}
		return newPipeline;
	}

	VkPipelineViewportStateCreateInfo Pipeline::buildViewportState() const
	{
		VkPipelineViewportStateCreateInfo viewportState{};
		static VkRect2D scissor = { 0, 0 }; //TODO: remove static
		scissor.offset = { 0, 0 };
		scissor.extent = { 1280, 720 };
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pNext = nullptr;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &_pipelineInfo._viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
		return viewportState;
	}

	VkPipelineColorBlendStateCreateInfo Pipeline::buildColorBlendState() const
	{
		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		static VkPipelineColorBlendAttachmentState colorBlendAttachment(
				VulkanInitializer::ColorBlendAttachmentState()); //TODO: remove static
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.pNext = nullptr;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		return colorBlending;
	}
}