/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PuzzleData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchelste <bchelste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/14 20:04:35 by bchelste          #+#    #+#             */
/*   Updated: 2022/09/18 22:05:36 by bchelste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <string>
#include <iostream>

class PuzzleData 
{
	public:
		
		PuzzleData();
		PuzzleData(int &newSize,  std::vector<int> &newTiles);
		virtual ~PuzzleData();

		int						getPuzzleSize() const;
		std::vector<int>		getTiles() const;

	private:
	
		int						puzzleSize;
		std::vector<int>		tiles;

};

std::ostream& operator<<(std::ostream &stream, const PuzzleData &puzzle);