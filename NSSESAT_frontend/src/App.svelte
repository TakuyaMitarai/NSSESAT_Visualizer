<button on:click={compileAndRunCpp}>実行</button>

<script>
	import { Scatter } from 'svelte-chartjs';
	import { Chart, LinearScale } from 'chart.js';
	import { PointElement } from 'chart.js';
	import { onMount } from 'svelte';

	Chart.register(PointElement);
	Chart.register(LinearScale);

	let dataFromAPI = {plotdata: []};
	let chartData = {
		datasets: [{
			label: 'データセット1',
			data: [],
			backgroundColor: 'rgba(75, 192, 192, 0.2)',
			borderColor: 'rgba(75, 192, 192, 1)',
			borderWidth: 1
		}]
	};

	let imageBase64 = "";

	async function compileAndRunCpp() {
        try {
            const response = await fetch("http://127.0.0.1:8000/compile_and_run_cpp", {
                method: 'POST'
            });
            if (response.ok) {
                const data = await response.json();
                console.log(data.message, data.output);
            } else {
                console.error(`API Error: ${response.statusText}`);
            }
        } catch (error) {
            console.error(`Fetch Error: ${error}`);
        }
    }
	
	async function sendPointData(point) {
		try {
			const response = await fetch("http://127.0.0.1:8000/set_point", {
				method: 'POST',
				headers: {
					'Content-Type': 'application/json'
				},
				body: JSON.stringify({ clicked_point: point })
			});
			if (response.ok) {
				const data = await response.json();
				console.log(data.message);
				imageBase64 = data.image;  // 画像データを取得
			} else {
				console.error(`API Error: ${response.statusText}`);
			}
		} catch (error) {
			console.error(`Fetch Error: ${error}`);
		}
	}

	// 既存のoptionsオブジェクト
	let options = {
		onClick: function(event, elements) {
			if (elements.length > 0) {
				const chartElement = elements[0];
				const dataIndex = chartElement.index;
				const datasetIndex = chartElement.datasetIndex;
				const clickedData = chartData.datasets[datasetIndex].data[dataIndex];
				const pointString = `${clickedData.x}-${clickedData.y}`;
				console.log("Clicked Point:", pointString);
				sendPointData(pointString);  // ここでデータを送信
			}
		}
	};

	async function fetchData() {
		try {
			const response = await fetch("http://127.0.0.1:8000/get_data");
			if (response.ok) {
				dataFromAPI = await response.json();
				chartData.datasets[0].data = dataFromAPI.plotdata;
				console.log("Fetched Data: ", dataFromAPI);
			} else {
				console.error(`API Error: ${response.statusText}`);
			}
		} catch (error) {
			console.error(`Fetch Error: ${error}`);
		}
	}

	onMount(fetchData);
</script>

<style>
	img {
		max-width: 40%;
		height: auto;
		margin: 0;  /* 画像のマージンを0に設定 */
	}
</style>

  
<div class="chart-container">
	{#if dataFromAPI.plotdata.length > 0}
		<Scatter data={chartData} {options} />
	{:else}
		<p>データがありません。</p>
	{/if}
</div>


{#if imageBase64}
  <img src={`data:image/png;base64,${imageBase64}`} alt="Visual representation of a tree structure" />
{:else}
{/if}
