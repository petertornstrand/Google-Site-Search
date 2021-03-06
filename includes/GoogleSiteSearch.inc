<?php
// $Id$

/**
 * Class for interaction with Google Site Search.
 */
class GoogleSiteSearch
{
	/**
	 * The Google key to use.
	 * @var string
	 */
	private $key = null;
	
	/**
	 * The search query.
	 * @var string
	 */
	private $query = null;
	
	/**
	 * Page size, number of results per page.
	 * @var int
	 */
	private $pageSize = 20;
	
	/**
	 * Current page in the results.
	 * @var int
	 */
	private $currentPage = null;
	
	/**
	 * Total results for a search.
	 * @var int
	 */
	private $totalResults = 0;
	
	/**
	 * The search results return from Google.
	 * @var SimpleXml
	 */
	private $searchResultsXml = null;
	
	/**
	 * Create a new instance of GoogleSiteSearch.
	 *
	 * @param string
	 */
	public function __construct($query, $key = null, $pageSize = 20)
	{
		$this->query = $query;
		$this->key = $key;
		$this->pageSize = $pageSize;
	}
	
	/**
	 * Set Google key.
	 *
	 * @param string $key The Google key.
	 * @return void
	 */
	public function SetKey($key)
	{
		$this->key = $key;
	}
	
	/**
	 * Get Google key
	 *
	 * @return string The Google key.
	 */
	public function GetKey()
	{
		return $this->key;
	}
	
	/**
	 * Set the page size.
	 *
	 * @param int $pageSize The new page size.
	 * @return void
	 */
	public function SetPageSize($pageSize)
	{
		$this->pageSize = $pageSize;
	}
	
	/**
	 * Get current page size.
	 *
	 * @return int The current page size.
	 */
	public function GetPageSize()
	{
		return $this->pageSize;
	}
	
	/**
	 * Get current page in the results.
	 *
	 * @return int The current page.
	 */
	public function GetCurrentPage()
	{
		return $this->currentPage;
	}
	
	/**
	 * Get total results for current search.
	 *
	 * @return int Total number of results.
	 */
	public function GetTotalResults()
	{
		return $this->totalResults;
	}
	
	/**
	 * Get total number of pages for search.
	 *
	 * @return int Total number of pages.
	 */
	public function GetTotalPages()
	{
		return ($this->totalResults / $this->pageSize + 1);
	}
	
	/**
	 * Get query current search.
	 *
	 * @return string The current query.
	 */
	public function GetQuery()
	{
		return $this->query;
	}
	
	/**
	 * Get search results.
	 *
	 * @param string $query The search query.
	 * @param int $page The page to get results from.
	 * @return array The search results.
	 * @throws Exception
	 */
	public function GetSearchResults($page = 1)
	{
		// set page
		$this->currentPage = $page;
		
		// calculate start postition based on page
		$startPos = ($this->currentPage - 1) * $this->pageSize;
		
		// assemble url
		$url = "http://www.google.com/search?start={$startPos}&num={$this->pageSize}&lr=lang_sv&client=google-csbe&output=xml_no_dtd&cx={$this->key}&q={$this->query}";
		
		// fetch results from google
		$results = simplexml_load_file($url);
		
		if($results !== false)
		{
			if(!isset($results->RES->M))
			{
				// no results, return null
				return null;
			}
			else
			{
				// save total results
				$this->totalResults = intval((string)$results->RES->M);
				
				// init results array
				$arr = array();
				
				// loop results
				foreach($results->RES->R as $result)
				{
					// init result array
					$item = array();
					$item['title'] = (string)$result->T;
					$item['url'] = (string)$result->UE;
					$item['description'] = (string)$result->S;
					$arr[] = $item;
				}
				
				// return results
				return $arr;
			}
		}
		else
		{
			throw new Exception('Could not load search results from Google.');
		}
	}
	
	/**
	 * Get a pager for the search results.
	 *
	 * @return string The pager HTML code.
	 */
	public function GetPager()
	{
		// get total pages
		$totalPages = $this->GetTotalPages();
		
		if($totalPages < 2)
		{
      return;
    }
    
		$html = "<div class=\"item-list\"><ul class=\"pager\">\n";
		
		$totalPages = $totalPages > 20 ? 20 : $totalPages;
		for($i=1; $i<=$totalPages; $i++)
		{
			if ($i != $this->currentPage) {
			 $html .= "\t<li class=\"pager-item\"><a href=\"?page={$i}\">{$i}</a></li>\n";
			}
			else {
			 $html .= "\t<li class=\"pager-current\">{$i}</li>\n";
			}
		}
		
		// close ol
		$html .= "</ul>\n";
		
		// return pager
		return $html;
	}
}